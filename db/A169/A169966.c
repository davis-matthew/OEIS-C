/*
 * A169966  Numbers whose decimal expansion contains only 0's and 3's.
 * https://oeis.org/A169966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers containing only digits 0 and 3 in decimal
 *            (a(1)=0, a(2)=3, a(3)=30, a(4)=33, …)
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 1024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)          /* offset zero */
        return (seq_t)0;

    idx_t rank = n - (idx_t)2;   /* 0‑based index among non‑zero terms */
    idx_t pow  = (idx_t)1;      /* pow = 2^(L-1) */
    idx_t L    = (idx_t)1;

    /* Find smallest L such that 2^(L-1) ≤ rank+1 < 2^L */
    while ((pow << 1) <= (rank + (idx_t)1)) {
        pow <<= 1;
        ++L;
    }

    /* offset inside the block of length L (0 … pow-1) */
    idx_t offset = rank - (pow - (idx_t)1);

    seq_t result = (seq_t)3;    /* most significant digit is always 3 */

    /* Append remaining L-1 digits determined by offset bits */
    for (idx_t i = 0; i < L - (idx_t)1; ++i) {
        idx_t bit = (offset >> (L - (idx_t)2 - i)) & (idx_t)1;
        seq_t digit = bit ? (seq_t)3 : (seq_t)0;
        result = result * (seq_t)10 + digit;
    }

    return result;
}
