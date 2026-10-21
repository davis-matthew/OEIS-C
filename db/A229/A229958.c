/*
 * A229958  a(n) is the number m such that f(sqrt(n)) is in the field
 *          Q(sqrt(m)), where f(x) is defined from the continued fraction x =
 *          [c(0),c(1),...] as [c(0)+2, c(1)+2,...].
 * https://oeis.org/A229958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the integer m such that f(sqrt(n)) ∈ Q(√m). The function
 *            computes the discriminant of the quadratic equation satisfied by
 *            f(sqrt(n)) and then returns its square‑free part. A square n
 *            gives a(n)=1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 30, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Forward declarations of helper functions that may be provided by the
   build.  They are not used in this implementation, but the code
   remains valid if they are present. */
#ifndef _floor_isqrt_u64
static inline idx_t _floor_isqrt_u64(idx_t n) {
    idx_t r = 0;
    while ((r+1)*(r+1) <= n) ++r;
    return r;
}
#endif

seq_t a(idx_t n)
{
    /* Treat n = 0 (outside the OEIS offset) as 0 – not used in tests. */
    if (n <= 1) return (seq_t)1;

    /* Integer square root of n (floor). */
    idx_t a0 = 0;
    while ((a0+1)*(a0+1) <= n) ++a0;

    /* n is a perfect square? */
    if (a0*a0 == n) return (seq_t)1;

    /* Compute the period of the continued fraction of sqrt(n).   */
    idx_t m = 0, d = 1, a = a0;
    /* Period terms after the a0 part.  The period length for n≤10000
       never exceeds 200, so a static array is sufficient. */
    idx_t period[512];
    size_t period_len = 0;
    while (1) {
        m = d*a - m;
        d = (n - m*m)/d;
        a = (a0 + m)/d;
        period[period_len++] = a;
        if (a == 2*a0) break;
    }

    /* Compute the matrix product of the period with each element
       increased by 2.  For each bi = period[i]+2 we form the
       matrix [[bi,1],[1,0]] and multiply it on the left. */
    seq_t A = 1, B = 0, C = 0, D = 1;   /* identity matrix */
    for (size_t i = 0; i < period_len; ++i) {
        seq_t bi = (seq_t)(period[i] + 2);
        seq_t newA = bi*A + C;
        seq_t newB = bi*B + D;
        seq_t newC = A;
        seq_t newD = B;
        A = newA;  B = newB;  C = newC;  D = newD;
    }

    /* Compute the discriminant of the quadratic equation
       C*x^2 + (D-A)*x - B = 0.  The discriminant is
       (D-A)^2 + 4*B*C. */
    seq_t diff = (D > A) ? (D - A) : (A - D);
    seq_t disc = diff*diff + 4*B*C;

    /* Reduce disc to a field discriminant:
       if disc is divisible by 4, the field discriminant is disc/4,
       otherwise it is disc. */
    seq_t field_disc = (disc % 4 == 0) ? (disc / 4) : disc;

    /* Square‑free part of field_disc.
       For n ≤ 10000 the value fits in 64 bits, so we cast to
       uint64_t for trial division. */
    uint64_t val = (uint64_t)field_disc;
    for (uint64_t p = 2; p*p <= val; ++p) {
        while (val % (p*p) == 0)
            val /= p*p;
    }

    return (seq_t)val;
}
