/*
 * A122756  Odd-indexed terms, a(n) = 2^n. Even-indexed terms, a(n) =
 *          floor(2^n+2^(n-1)).
 * https://oeis.org/A122756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0)=1, a(1)=2, a(2)=6, a(n)=4*a(n-2) for n>=3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All computations are performed in seq_t, with idx_t as the index type. */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 6;

    /* n >= 3 */
    idx_t k = n / 2;          /* floor(n/2) */
    if (n & 1U) {             /* odd n: a(2k+1) = 2 * 4^k = 2^n */
        seq_t pow4 = seq_pow((seq_t)4, k);   /* 4^k */
        return (seq_t)2 * pow4;
    } else {                  /* even n: a(2k) = 6 * 4^(k-1) */
        seq_t pow4 = seq_pow((seq_t)4, k - 1); /* 4^(k-1) */
        return (seq_t)6 * pow4;
    }
}
