/*
 * A031780  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly 12 ones.
 * https://oeis.org/A031780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2955, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A031780 – the nth smallest integer k such that the period of the continued
 * fraction for sqrt(k) contains exactly 12 ones. The sequence is indexed starting at 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root (floor) for 64‑bit values using binary search. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFull;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* Test whether sqrt(k) has exactly 12 entries equal to 1 in its period. */
static inline bool has12ones(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;              /* perfect square */

    idx_t m = 0, d = 1, a = a0;
    int ones = 0;

    for (;;) {
        /*   m' = d * a - m   */
        unsigned __int128 tmp1 = (unsigned __int128)d * a - m;
        idx_t m1 = (idx_t)tmp1;

        /*   d' = (k - m'^2) / d   */
        unsigned __int128 diff = (unsigned __int128)k - (unsigned __int128)m1 * m1;
        idx_t d1 = (idx_t)(diff / d);

        /*   a' = (a0 + m') / d'   */
        idx_t a1 = (a0 + m1) / d1;

        if (a1 == 1) {
            if (++ones > 12) return false;      /* too many ones */
        }

        if (a1 == 2 * a0) {                    /* period finished */
            return ones == 12;
        }

        m = m1;
        d = d1;
        a = a1;
    }
}

/* Internal storage for already found terms. */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static idx_t  next_k     = 1;   /* next integer to test */

/* Append a new term to the dynamic array. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 128;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n <= 0 */

    while (terms_len < (size_t)n) {
        /* Scan successive integers for the next qualifying k. */
        for (;;) {
            if (has12ones(next_k)) {
                append_term((seq_t)next_k);
                ++next_k;
                break;
            }
            ++next_k;
        }
    }
    return terms[n - 1];
}
