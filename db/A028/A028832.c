/*
 * A028832  Number of distinct integers in period of continued fraction for
 *          sqrt(n), or 0 if n is a square.
 * https://oeis.org/A028832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of distinct integers in the period of the continued
    fraction expansion of √n; a(n)=0 if n is a perfect square.  */

/*  The algorithm follows the standard recurrence for the
    continued fraction of a square root:   m0=0, d0=1, a0=floor(√n)
    mn+1 = d_n · a_n – m_n
    d_n+1 = (n – (m_n+1)^2)/d_n
    a_n+1 = floor((a0 + m_n+1)/d_n+1)
    The period ends when a_n+1 = 2·a0.  Count the distinct a_i (i≥1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- integer square root for 64‑bit indices ---------------------------------*/
static inline idx_t int_sqrt_u64(idx_t n) {
    idx_t low  = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq <  n) low = mid + 1;
        else         high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

/*--- main sequence function -------------------------------------------------*/
seq_t a(idx_t n) {
    /* 0 if n is a perfect square. */
    idx_t r = int_sqrt_u64(n);
    if (r * r == n) return (seq_t)0;

    idx_t a0 = r;       /* integer part of √n */
    idx_t m  = 0;
    idx_t d  = 1;
    idx_t a_prev = a0;  /* a0 is not part of the period */

    /* Allocate an array to remember which a-values have appeared.
       The values are in the range [1, 2*a0], because the first value
       that appears after the period ends is 2*a0. */
    idx_t seen_size = 2 * a0 + 1;
    /* calloc returns void*, which is implicitly convertible in C
       but needs a cast in C++.  The cast is safe because we allocate
       byte-sized booleans. */
    bool *seen = (bool *)calloc(seen_size, sizeof(bool));
    if (!seen) return (seq_t)0;  /* allocation failure -> 0 */

    seq_t distinct = 0;

    for (;;) {
        /* next state using 128‑bit arithmetic to avoid overflow */
        __uint128_t prod = (__uint128_t)d * a_prev;
        m = (idx_t)(prod - m);

        __uint128_t sq  = (__uint128_t)m * m;
        d = (idx_t)((n - (idx_t)sq) / d);

        a_prev = (idx_t)((a0 + m) / d);  /* this is a_i of the period */

        /* Record the new a_i if it hasn't appeared before. */
        bool *slot = &seen[a_prev];
        if (!*slot) {
            *slot = true;
            distinct++;
        }

        if (a_prev == 2 * a0) break;  /* period completes */
    }

    free(seen);
    return distinct;
}
