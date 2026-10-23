/*
 * A022392  Fibonacci sequence beginning 1, 22.
 * https://oeis.org/A022392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0)=1, a(1)=22
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci: returns F(n) for non‑negative n */
static seq_t fib_fast(idx_t n)
{
    seq_t a = 0; /* F(0) */
    seq_t b = 1; /* F(1) */

    /* Find the index of the most significant set bit in n */
    int msb = 0;
    for (int i = 63; i >= 0; --i)
        if ((n >> i) != 0) { msb = i; break; }

    for (int i = msb; i >= 0; --i) {
        /* Compute F(2k) and F(2k+1) from F(k)=a and F(k+1)=b */
        seq_t t1 = a * ((seq_t)2 * b - a); /* F(2k)   */
        seq_t t2 = a * a + b * b;           /* F(2k+1) */

        if ((n >> i) & 1) {
            a = t2;          /* F(2k+1) => new a */
            b = t1 + t2;     /* F(2k+2) => new b */
        } else {
            a = t1;          /* F(2k)   => new a */
            b = t2;          /* F(2k+1) => new b */
        }
    }
    return a; /* F(n) */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;

    /* a(n) = a(0)*F(n-1) + a(1)*F(n) for n >= 2 */
    seq_t fn_minus1 = fib_fast(n - 1);
    seq_t fn = fib_fast(n);
    return fn_minus1 + (seq_t)22 * fn;
}
