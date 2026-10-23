/*
 * A165415  a(n) = the smallest positive integer that contains more digits
 *          written in binary than n has written in binary, and which does not
 *          contain binary n as a substring in its binary representation.
 * https://oeis.org/A165415
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) : the smallest positive integer that has a longer binary
   representation than n and does not contain binary n as a substring.
   For n < 2 the function returns 0 (the sequence is defined for n ≥ 2). */

#include <stdint.h>

/* Determine the number of bits needed to represent x (x > 0). */
static inline idx_t bitlen(idx_t x)
{
    idx_t len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2 */

    idx_t k = bitlen(n);                 /* length of binary n */
    idx_t start = ((idx_t)1 << k);       /* smallest number with k+1 bits */
    idx_t mask  = ((idx_t)1 << k) - 1;   /* lower k bits mask */

    idx_t x = start;
    for (;;) {                           /* at least one valid x exists */
        if (((x >> 1) != n) && ((x & mask) != n))
            return (seq_t)x;
        ++x;
    }
}
