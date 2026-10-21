/*
 * A079945  Partial sums of A079882.
 * https://oeis.org/A079945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) =
 *            (3*n+3-2^A000523((n+2)/2)-(-1)^A079944(n)*(n+3-3*2^A000523((n+2)/2)))/2
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the floor of log2 for a 64‑bit integer */
static inline unsigned int floor_log2_u64(uint64_t v)
{
    unsigned int r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* m = floor((n+2)/2) */
    idx_t m = (n + 2) >> 1;

    /* c = floor_log2(m) = A000523(m) */
    unsigned int c = 0;
    if (m > 0)
        c = floor_log2_u64(m);

    /* pow2_c = 2^c */
    seq_t pow2_c = ((seq_t)1) << c;

    /* Determine A079944(n) = b (0 or 1) */
    bool b;
    if (m == 1) {            /* m==1 occurs only for n=0 and n=1   */
        b = (n & 1ULL) != 0; /* for n>=1 we need b=1, n=0 b is unused */
    } else {
        /* b = 1 iff m >= 3*2^(c-1) */
        if (c == 0)
            b = true;
        else
            b = (m >= (3ULL << (c - 1)));
    }

    if (!b) {
        /* a(n) = n + 2^c */
        return (seq_t)n + pow2_c;
    } else {
        /* a(n) = 2n + 3 - 2^(c+1) */
        seq_t two_n_plus3 = (seq_t)(2 * n + 3);
        seq_t pow2_c1 = pow2_c << 1;   /* 2^(c+1) */
        return two_n_plus3 - pow2_c1;
    }
}
