/*
 * A130589  a(n) = F(F(n)-1), where F(n) = A000045(n) (the Fibonacci numbers).
 * https://oeis.org/A130589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(F(n)-1), with F(0)=0, F(1)=1
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

/* Fast doubling Fibonacci for an index of type seq_t.
   It works for any non‑negative big integer index. */
static seq_t fib_big(seq_t n)
{
    seq_t a = 0;    /* F(0) */
    seq_t b = 1;    /* F(1) */
    size_t bits = bitlen_seq_t(n);
    /* Process bits from most significant to least */
    for (size_t i = 0; i < bits; ++i)
    {
        seq_t t1 = a * ((b << 1) - a);   /* F(2k)   */
        seq_t t2 = a * a + b * b;        /* F(2k+1) */
        if ((n >> (bits - 1 - i)) & 1)
        {
            a = t2;          /* F(2k+1) */
            b = t1 + t2;     /* F(2k+2) */
        }
        else
        {
            a = t1;          /* F(2k)   */
            b = t2;          /* F(2k+1) */
        }
    }
    return a;
}

/* Sequence a(n) = F(F(n)-1) */
seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* F(-1)=1 by Fibonacci extension */
    seq_t fn = fib_big((seq_t)n);      /* F(n)  */
    seq_t m  = fn - 1;                 /* F(n)-1, non‑negative for n≥1 */
    return fib_big(m);                 /* F(F(n)-1) */
}
