/*
 * A204058  Largest integer m such that both m and n*m are decimal pandigital
 *          numbers (A050278).
 * https://oeis.org/A204058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 9, u128 n <= 9, big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   digit_frequencies, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = maximum m such that m and n*m are decimal pandigital numbers. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"
#include "../utils/mem.h"

/* Recursive generator of pandigital numbers with first digit non‑zero. */
static void gen_pandigits(int pos, uint64_t cur,
                          bool used[10], seq_t *arr, size_t *idx)
{
    if (pos == 10) {
        arr[*idx] = (seq_t)cur;
        ++(*idx);
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (!used[d] && !(pos == 0 && d == 0)) {
            used[d] = true;
            gen_pandigits(pos + 1, cur * 10 + d, used, arr, idx);
            used[d] = false;
        }
    }
}

static seq_t *pandigits = NULL;
static size_t pandigits_len = 0;

seq_t a(idx_t _n)
{
    /* Build the list once */
    if (pandigits == NULL) {
        pandigits_len = 3265920; /* 9 × 9! */
        pandigits = mem_new(seq_t, pandigits_len);
        bool used[10] = {false};
        size_t idx = 0;
        gen_pandigits(0, 0ULL, used, pandigits, &idx);
        /* idx should now equal pandigits_len */
    }

    if (_n == 0) return 0; /* safe guard, offset is 1 */

    for (size_t i = pandigits_len; i > 0; --i) {
        seq_t m = pandigits[i - 1];
        seq_t prod = (seq_t)_n * m;
        if (prod > (seq_t)9876543210ULL) continue; /* cannot be pandigital */

        unsigned int freq[10];
        digit_frequencies(prod, freq);
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] != 1) { ok = false; break; }
        }
        if (ok) return m; /* found the maximum m */
    }
    return 0; /* if none found */
}
