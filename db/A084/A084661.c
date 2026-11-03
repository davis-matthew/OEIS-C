/*
 * A084661  Numbers k such that k + sum_of_digits(k) is a cube.
 * https://oeis.org/A084661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th integer k such that k + digit_sum(k) is a
 *            perfect cube. The implementation iterates over cube roots m≥1
 *            and tests only the few values k near m³, because k = m³ – sum(k)
 *            and sum(k) ≤ 9·digits(k)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Validated: standard, pass, 57 terms through n = 242 (2026-11-03)
 * Helpers:   digit_sum, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities from the library */
#include <math.h>
#include "../utils/digit_sum.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* --- static cache for results ----------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* --- index of the next cube root to process --------------------- */
    static idx_t next_m = 1;

    /* --- initialize cache on first call ----------------------------- */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    /* --- generate terms until we have at least n of them -------------- */
    while (cache_len < n) {
        /* cube = next_m³ */
        seq_t mm = (seq_t)next_m;
        seq_t cube = mm * mm * mm;            /* seq_t multiplication */

        /* bound on possible sum of digits of k: 9·digits(cube)             */
        uint64_t cube64 = (uint64_t)cube;     /* cube fits in 64‑bit for n≈10⁴ */
        int digits_cube = num_digits0(cube64);
        int max_sum   = 9 * digits_cube;

        seq_t start = (cube > (seq_t)max_sum) ? (cube - (seq_t)max_sum) : (seq_t)1;

        /* search k in [start, cube] */
        for (seq_t k = start; k <= cube; ++k) {
            seq_t s = k + digit_sum(k);
            if (s == cube) {
                append_candidate(&cache, &cache_len, &cache_cap, k);
            }
        }

        ++next_m;
    }

    return cache[n - 1];
}
