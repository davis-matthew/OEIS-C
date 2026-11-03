/*
 * A163235  Two-dimensional Binary Reflected Gray Code, transposed version:
 *          a(i,j) = bits of binary expansion of A003188(j) interleaved with
 *          that of A003188(i).
 * https://oeis.org/A163235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = interleave bits of Gray(n, i) and Gray(n, j) where (i, j)
 *            are the antidiagonal coordinates of index n. Even bit positions
 *            come from Gray(i), odd positions from Gray(j)
 * Range:     u8 n <= 135, u16 n <= 2079, u32 n <= 2079, u64 n <= 2079,
 *            u128 n <= 2079, big any n
 * Validated: standard, pass, 85 terms through n = 1952 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* uint*_t, __uint128_t */
#include <limits.h>  /* CHAR_BIT */

/* Find k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2 */
static inline idx_t antidiagonal_k(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponentially grow high until it exceeds n */
    while (((__uint128_t)high * (high + 1)) / 2 <= n) {
        low  = high;
        high <<= 1;          /* double high */
    }
    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((__uint128_t)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;              /* largest k with t_k <= n */
}

seq_t a(idx_t n)
{
    /* Locate the antidiagonal and cell (i, j) */
    idx_t k = antidiagonal_k(n);
    idx_t t_k = (idx_t)((( __uint128_t)k * (k + 1)) / 2);
    idx_t i = n - t_k;
    idx_t j = k - i;            /* i + j = k */

    /* Gray codes of i and j */
    seq_t gray_i = (seq_t)i ^ ((seq_t)i >> 1);
    seq_t gray_j = (seq_t)j ^ ((seq_t)j >> 1);

    /* Interleave bits: even positions ← Gray(i), odd positions ← Gray(j) */
    seq_t res = 0;
    idx_t  shift = 0;                        /* bit position in result */

    /* Total number of bits representable in seq_t */
    const idx_t width = (idx_t)(sizeof(seq_t) * CHAR_BIT);

    seq_t ti = gray_i;
    seq_t tj = gray_j;

    while (ti || tj) {
        /* Stop if shifting beyond the width of seq_t */
        if (shift + 1 >= width) break;

        if (ti & 1) res |= ((seq_t)1 << shift);
        shift++;

        if (tj & 1) res |= ((seq_t)1 << shift);
        shift++;

        ti >>= 1;
        tj >>= 1;
    }

    return res;
}
