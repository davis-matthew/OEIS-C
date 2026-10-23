/*
 * A189939  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sin(Pi/8), t=cos(Pi/8).
 * https://oeis.org/A189939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   c(n) = n + ⌊n·sec(π/8)⌋ + ⌊n·tan(π/8)⌋
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed constants (long double for maximum precision) */
    static const long double pi      = acosl(-1.0L);
    static const long double tan_pi8 = sqrtl(2.0L) - 1.0L;               /* tan(π/8)   */
    static const long double sec_pi8 = 2.0L / sqrtl(2.0L + sqrtl(2.0L));  /* sec(π/8)   */

    /* Compute the floor parts using long double arithmetic */
    unsigned long long floor_sec = (unsigned long long)floorl((long double)n * sec_pi8);
    unsigned long long floor_tan = (unsigned long long)floorl((long double)n * tan_pi8);

    /* Convert to seq_t and return the result */
    return (seq_t)n + (seq_t)floor_sec + (seq_t)floor_tan;
}
