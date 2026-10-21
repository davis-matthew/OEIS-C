/*
 * A065157  Table of binary string substitutions: a(i,j) is obtained by
 *          substituting i for each 1-bit in j.
 * https://oeis.org/A065157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A065157: table of binary string substitutions.
   The sequence is indexed starting at n=1.
   For each integer n, determine the pair (i,j) as follows:
     - Let s = i + j be the smallest integer such that
           (s-1)*(s-2)/2 < n ≤ (s-1)*s/2 .
     - Order the pairs on this anti‑diagonal by decreasing i:
           (s-1,1), (s-2,2), … , (1,s-1).
     - The offset offset = n − (s-1)*(s-2)/2 − 1 gives the
       zero‑based position on the diagonal, so
           i = (s-1) − offset,  j = s − i .
   a(i,j) is obtained by replacing every 1‑bit of j by the binary
   representation of i and every 0‑bit by a single 0, concatenating
   the resulting bits, and interpreting the result as a binary number.
   The function returns a(i,j) as a seq_t value.

   No predefined tables are used; the computation is O(bits(i)+bits(j)).  */

#include <stdint.h>

/* Return number of bits needed to represent v (>0). */
static unsigned bitlen_u64(uint64_t v)
{
    unsigned len = 0;
    while (v) { ++len; v >>= 1; }
    return len;
}

/* Compute a(i,j) – the substitution table entry. i,j ≥ 1. */
static seq_t table_entry(idx_t ii, idx_t jj)
{
    unsigned ilen = bitlen_u64((uint64_t)ii);
    unsigned jlen = bitlen_u64((uint64_t)jj);
    seq_t res = (seq_t)0;
    for (int pos = (int)jlen - 1; pos >= 0; --pos) {
        if ((jj >> pos) & 1ULL) {
            res = (res << ilen) | (seq_t)ii;
        } else {
            res <<= 1;
        }
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence; for n==0 we return 0. */
    if (n == 0) return (seq_t)0;

    /* Find the anti‑diagonal number s such that the diagonal
       contains the n-th entry. */
    idx_t s = 2;
    while ((s * (s - 1)) / 2 < n) ++s;

    /* Number of entries preceding this diagonal. */
    idx_t prev = (s - 1) * (s - 2) / 2;
    /* 0‑based position within the diagonal. */
    idx_t offset = n - prev - 1;

    /* Determine pair (i,j) in decreasing i order. */
    idx_t i = (s - 1) - offset;
    idx_t j = s - i;

    return table_entry(i, j);
}
