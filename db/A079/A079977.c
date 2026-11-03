/*
 * A079977  Fibonacci numbers interspersed with zeros.
 * https://oeis.org/A079977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(k+1) if n = 2k, a(n) = 0 otherwise. Here
 *            A000045(k+1) is the (k+1)-th Fibonacci number with F(1)=1,
 *            F(2)=1. This implementation uses a fast‑doubling algorithm to
 *            compute F(k+1) for k = n/2
 * Range:     u8 n <= 25, u16 n <= 47, u32 n <= 93, u64 n <= 185,
 *            u128 n <= 371, big any n
 * Validated: standard, pass, 77 terms through n = 989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the (k+1)-th Fibonacci number (F(1)=1, F(2)=1) using
   iterative fast‑doubling.  The parameter k is unsigned, and
   k=0 returns F(1)=1.  The function works for any seq_t type
   that supports +, -, *, and cast from integer literals. */
static inline seq_t fib_k_plus_one(idx_t k)
{
    if (k == 0)
        return (seq_t)1;                 /* F(1) */

    /* (a,b) will hold (F_n, F_{n+1}) during the iteration.  */
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* Process bits of k from most significant to least  */
    idx_t msb = 63 - __builtin_clzll(k);  /* number of bits in k minus 1 */

    for (long i = msb; i >= 0; --i)
    {
        /* Compute: d = F_{2n} = a * (2*b - a)
                   e = F_{2n+1} = a*a + b*b                           */
        seq_t two_b_minus_a = (b + b) - a;   /* 2*b - a  */
        seq_t d = a * two_b_minus_a;          /* F_{2n}   */
        seq_t e = a * a + b * b;              /* F_{2n+1} */

        if (((k >> i) & 1) == 0)
        {
            a = d;
            b = e;
        }
        else
        {
            a = e;
            b = d + e;
        }
    }
    /* After the loop, b == F_{k+1}.  */
    return b;
}

/* The sequence function a(n) for OEIS A079977.  */
seq_t a(idx_t n)
{
    if ((n & 1u) != 0)
        return (seq_t)0;               /* odd index → zero */

    idx_t k = n >> 1;                 /* n = 2k */
    return fib_k_plus_one(k);
}
