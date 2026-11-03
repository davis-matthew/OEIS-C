/*
 * A054032  Numbers n such that n^2 contains exactly 4 different digits.
 * https://oeis.org/A054032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute the nth integer n such that n^2 contains exactly 4
 *            distinct decimal digits
 * Range:     u8 n <= 110, u16 n <= 3130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9531 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the sequence is defined by a property of n^2; no closed form is known,
   so the implementation uses a simple incremental search with memoisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* helper to count distinct decimal digits of a 128‑bit number */
static unsigned
count_distinct_digits(unsigned __int128 v)
{
    unsigned mask = 0;
    if (v == 0) return 1;
    while (v > 0) {
        int d = v % 10;
        mask |= 1u << d;
        v /= 10;
    }
    return __builtin_popcount(mask);
}

static seq_t  *terms = NULL;     /* stored terms of the sequence   */
static size_t terms_len = 0;     /* current number of stored terms */
static size_t terms_cap = 0;     /* current allocated capacity   */
static uint64_t next_candidate = 1;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 – return 0 by convention */

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* search for the next candidate whose square has 4 distinct digits */
        while (1) {
            uint64_t cand = next_candidate++;
            unsigned __int128 sq = (unsigned __int128)cand * cand;
            if (count_distinct_digits(sq) == 4) {
                /* ensure capacity and store the found term */
                grow_seq(&terms, &terms_cap, terms_len + 1);
                terms[terms_len++] = (seq_t)cand;
                break;
            }
        }
    }
    return terms[n - 1];
}
