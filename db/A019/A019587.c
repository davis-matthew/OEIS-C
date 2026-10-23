/*
 * A019587  The left budding sequence: number of i such that 0 < i <= n and 0
 *          < {phi*i} <= {phi*n}, where {} denotes the fractional part and phi
 *          = A001622.
 * https://oeis.org/A019587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = #{i : 1 ≤ i ≤ n, 0 < {φ·i} ≤ {φ·n}} (φ = (1+√5)/2)
 * Range:     u8 n <= 274, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* φ in long double precision */
    const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;

    /* fractional part of φ·n */
    long double r = phi * (long double)n;
    r -= floorl(r);

    idx_t i;
    uint64_t count = 0;
    for (i = 1; i <= n; ++i) {
        long double val = phi * (long double)i;
        val -= floorl(val);
        /* add a negligible epsilon to correctly count the case val == r (i = n) */
        if (val <= r + 1e-18L) {
            ++count;
        }
    }
    return (seq_t)count;
}
