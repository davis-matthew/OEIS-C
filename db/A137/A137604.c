/*
 * A137604  Consider the sequence: b(0) = n, and for k >= 1, b(k) = b(k-1)/2
 *          if b(k-1) is even, otherwise b(k) = n - (b(k-1)+1)/2. Then a(n) =
 *          (Sum_{0<=k<=m} b(k)) - 1 for n > 1, where m is the smallest index
 *          such that b(m) = 1; a(1) = 1.
 * https://oeis.org/A137604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137604: a(n) = sum_{k=0..m} b(k) - 1,
   where b(0)=n,
   b(k)=b(k-1)/2 if b(k-1) even,
   else b(k)=n - (b(k-1)+1)/2,
   and m is the first index with b(m)=1; a(1)=1. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    const seq_t sn = (seq_t)n;
    seq_t sum = 0;
    seq_t b   = sn;

    while (1) {
        sum += b;
        if (b == (seq_t)1)
            break;

        /* Test evenness using modulus 2 */
        seq_t rem = b % (seq_t)2;
        if (rem == (seq_t)0) {
            /* even */
            b = b / (seq_t)2;
        } else {
            /* odd */
            b = sn - ((b + (seq_t)1) / (seq_t)2);
        }
    }

    return sum - (seq_t)1;
}
