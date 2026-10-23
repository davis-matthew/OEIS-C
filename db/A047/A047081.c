/*
 * A047081  a(n) = Sum_{k=0..n} T(n, k), array T as in A047080.
 * https://oeis.org/A047081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000073(n+1) + A000073(n+2) (Tribonacci numbers with
 *            T(0)=0, T(1)=0, T(2)=1)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* T(0)=0, T(1)=0, T(2)=1 */
    seq_t t0 = 0;   // T(i-2)
    seq_t t1 = 0;   // T(i-1)
    seq_t t2 = 1;   // T(i)
    idx_t limit = n + 2;   // we need T(n+1) and T(n+2)

    /* iterate from i = 3 up to limit inclusive */
    for (idx_t i = 3; i <= limit; ++i) {
        seq_t next = t2 + t1 + t0;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    /* t1 == T(limit-1) == T(n+1), t2 == T(limit) == T(n+2) */
    return t1 + t2;
}
