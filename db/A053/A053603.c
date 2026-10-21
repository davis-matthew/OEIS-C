/*
 * A053603  Number of ways to write n as an ordered sum of two nonzero
 *          triangular numbers.
 * https://oeis.org/A053603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (t_i , t_j) of positive *
 *            triangular numbers T_k = k*(k+1)/2 such that t_i + t_j = n. *
 *            Implements `seq_t a(idx_t n)` with O(sqrt(n)) time
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for unsigned 128‑bit values. */
static inline idx_t int128_sqrt (__int128 val)
{
    idx_t low = 0;
    /* high is set to 2^63‑1 which is > sqrt(2^127). */
    idx_t high = ((idx_t)1 << 63) - 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 == val) return mid;
        if (mid2 < val) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Test whether x is a triangular number. */
static inline int is_tri (idx_t x) {
    __int128 val = (__int128)8 * x + 1;      /* D = 1 + 8x */
    idx_t r = int128_sqrt (val);
    if ((__int128)r * r != val) return 0;    /* not a perfect square */
    return (r & 1) == 1;                    /* r must be odd so (-1+r)/2 integer */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* a(0)=a(1)=0 */

    seq_t n_seq = (seq_t)n;                 /* for arithmetic in seq_t */
    unsigned long long cnt = 0;             /* count may exceed seq_t limits */

    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;  /* triangular number T_k */
        idx_t t_i = (idx_t)t;
        if (t_i > n - 1) break;               /* t_j would be <=0 */
        idx_t t_j = n - t_i;                  /* second summand */
        if (is_tri (t_j)) ++cnt;
    }

    return (seq_t)cnt;                       /* cast back to seq_t */
}
