/*
 * A190084  n + [n*r/t] + [n*s/t]; r=1, s=sin(2*Pi/5), t=csc(2*Pi/5).
 * https://oeis.org/A190084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sin(2π/5)) + floor(n*sin^2(2π/5))
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static const long double ALPHA = 0.9510565162951535836186L; /* sin(72°)   */
static const long double BETA  = 0.9045084971874730696829L; /* sin^2(72°) */

/* External linkage, as required */
seq_t a(idx_t n)
{
    /* Convert n to long double for accurate multiplication */
    long double nd = (long double)n;

    /* Compute the two floor terms via long double arithmetic.
       The intermediate product is snapped to uint64_t via truncation,
       which yields the correct floor because the irrational multiples
       never hit an integer. */
    uint64_t t1 = (uint64_t)floorl(nd * ALPHA);
    uint64_t t2 = (uint64_t)floorl(nd * BETA);

    /* Assemble the result. Cast parts to seq_t before summing to avoid
       overflow in smaller seq_t types (e.g., uint8_t). */
    seq_t result = (seq_t)n;
    result += (seq_t)t1;
    result += (seq_t)t2;
    return result;
}
