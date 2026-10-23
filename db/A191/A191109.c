/*
 * A191109  a(1)=1, and if x is a term then 3x-1 and 3x+2 are terms too.
 * https://oeis.org/A191109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191109
   Sequence consisting of the numbers whose base‑3 representation
   contains only the digits 1 and 2 and ends in 2, together with the
   initial term 1.  Indexed from n=1: a(1)=1, a(2)=2, a(3)=5, a(4)=8, … */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the single special term */
    if (n == 1) return (seq_t)1;

    /* Find the smallest L such that 2^L >= n.
       For n=2^k the block of length L=k is needed.         */
    idx_t pow2 = 1;      /* 2^0 */
    int     L    = 0;     /* exponent */

    while (pow2 < n) {
        pow2 <<= 1;  /* now pow2 = 2^(L+1) */
        ++L;
    }

    /* For n=1 we would get L=0; treat that case separately above.
       For n=2 we get L=1 which corresponds to the block of length 1
       (the numbers 1 and 2).  For L>=2 the block contains 2^(L-1)
       numbers, all ending with the digit 2 in base 3.           */
    if (L == 1) {   /* block of length 1: 1, 2 */
        return (n == 1) ? (seq_t)1 : (seq_t)2;
    }

    /* Compute the offset within the block of length L.   */
    idx_t prev   = ((idx_t)1 << (L-1));            /* 2^(L-1) */
    idx_t offset = n - prev;                       /* 1 … 2^(L-1) */
    idx_t k      = offset - 1;                     /* 0‑based index      */

    /* Build the number in base 3: digits d_{L-1} … d_1 followed by
       the final digit 2.  Each d_i is 1 if the corresponding bit of
       k is 0, otherwise 2.                                            */
    seq_t value = 0;
    for (int pos = L-1; pos >= 1; --pos) {
        int bit   = (k >> (pos-1)) & 1;
        seq_t digit = (seq_t)(1 + bit);     /* 1 or 2 */
        value = value * (seq_t)3 + digit;
    }
    /* Append the mandatory final digit 2. */
    value = value * (seq_t)3 + (seq_t)2;

    return value;
}
