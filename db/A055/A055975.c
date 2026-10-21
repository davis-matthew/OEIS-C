/*
 * A055975  First differences of A003188 (decimal equivalent of the Gray
 *          Code).
 * https://oeis.org/A055975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign, mult
 * Formula:   a(n) = 2^v₂(n) * (-1)^((n/2^v₂(n)-1)/2) where v₂(n) is the
 *            exponent of the largest power of 2 dividing n
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* external types provided by the build */
seq_t a(idx_t n)
{
    /* the sequence is defined for n≥1; behaviour for 0 is arbitrary */
    if (n == 0)
        return 0;

    /* 1. compute the largest power of two dividing n: 2^p */
    /* trailing‑zero count of n – works for idx_t up to 64 bits */
    int p = __builtin_ctzll((unsigned long long)n);   /* p = v₂(n) */

    /* 2. odd component m = n / 2^p    (m is always odd) */
    idx_t m = n >> p;

    /* 3. sign factor: (-1)^((m-1)/2)  */
    /* (m-1)/2 is an integer because m is odd */
    int exp = (int)((m - 1) / 2);
    int sign = (exp & 1) ? -1 : 1;

    /* 4. compute 2^p in seq_t */
    seq_t power = (seq_t)1;
    power <<= p;                 /* power = 2^p */

    /* 5. apply sign */
    return (sign > 0) ? power : -power;
}
