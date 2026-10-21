/*
 * A213626  a(n) = Sum_{0<=i<j<k<=n} b(i)*b(j)*b(k), where b(m) = A020985(m).
 * https://oeis.org/A213626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 40, i16 n <= 2216, i32 n <= 9567, i64 n <= 9567,
 *            i128 n <= 9567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A213626
 *
 * The function `a(n)` computes
 *
 *   a(n) = Σ_{0 ≤ i < j < k ≤ n} b(i) * b(j) * b(k)
 *
 * where b(n) = (-1)^{popcount(n & (n >> 1))}.  The exponent is the
 * number of adjacent `1` bits in the binary expansion of n.  This
 * recursion yields
 *
 *   a(n) = a(n-1) + b(n) * S2(n-1)
 *   S2(n) = S2(n-1) + b(n) * S1(n-1)
 *   S1(n) = S1(n-1) + b(n)
 *
 * with S1( -1 ) = S2( -1 ) = a( -1 ) = 0.  The function is O(n)
 * and works for any n ≥ 0.  All arithmetic is performed in `seq_t`,
 * which may be a built‑in signed integer type or an arbitrary‑precision
 * integer class.
 */

#include <stdint.h>

static inline unsigned int popcount_u64(unsigned long long x)
{
    unsigned int c = 0;
    while (x)
    {
        c += (unsigned int)(x & 1ULL);
        x >>= 1;
    }
    return c;
}

static inline seq_t b_val(idx_t n)
{
    unsigned long long x = (unsigned long long)n;
    unsigned long long y = x & (x >> 1);
    unsigned int pc = popcount_u64(y);
    return ((pc & 1) == 0) ? (seq_t)1 : (seq_t)-1;
}

seq_t a(idx_t n)
{
    seq_t S1 = 0, S2 = 0, a_val = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t bi = b_val(i);
        a_val = a_val + bi * S2;
        S2 = S2 + bi * S1;
        S1 = S1 + bi;
    }
    return a_val;
}
