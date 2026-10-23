/*
 * A041156  Numerators of continued fraction convergents to sqrt(88).
 * https://oeis.org/A041156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 394*a(n-6) - a(n-12); Initial values (n = 0 … 11):
 *            a(0)=9, a(1)=19, a(2)=28, a(3)=47, a(4)=75, a(5)=197, a(6)=3621,
 *            a(7)=7439, a(8)=11060, a(9)=18499, a(10)=29559, a(11)=77617
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static const seq_t init[12] = {
    /* 0 */  9ULL,
    /* 1 */ 19ULL,
    /* 2 */ 28ULL,
    /* 3 */ 47ULL,
    /* 4 */ 75ULL,
    /* 5 */ 197ULL,
    /* 6 */ 3621ULL,
    /* 7 */ 7439ULL,
    /* 8 */ 11060ULL,
    /* 9 */ 18499ULL,
    /*10 */ 29559ULL,
    /*11 */ 77617ULL
};

seq_t a(idx_t n)
{
    /* For small n return the known initial value. */
    if (n < 12)
        return init[(size_t)n];

    /* Sliding window of the last 12 terms. */
    seq_t window[12];
    for (size_t i = 0; i < 12; ++i)
        window[i] = init[i];

    /* Compute terms up to n. */
    for (idx_t i = 12; i <= n; ++i)
    {
        /* Indices of terms i-6 and i-12 within the circular buffer. */
        size_t idx6  = (size_t)((i - 6) % 12);
        size_t idx12 = (size_t)((i - 12) % 12);

        /* a(i) = 394*a(i-6) - a(i-12). */
        seq_t term = ((seq_t)394 * window[idx6]) - window[idx12];
        window[(size_t)(i % 12)] = term;
    }

    return window[(size_t)(n % 12)];
}
