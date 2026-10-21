/*
 * A074739  Numbers n such that omega(n) > log(log(n)).
 * https://oeis.org/A074739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Enumerates numbers n such that ω(n) > log(log(n))
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For any n ≥ 1 a(n) is the n‑th smallest integer with this property. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for 0, but return 0 to keep the function well‑defined. */
        return (seq_t)0;
    }

    idx_t count = 0;   /* how many qualifying numbers have been found */
    idx_t x = 1;       /* current integer to test */

    while (count < n) {
        ++x;
        /* compute Ω(x) : number of distinct prime factors */
        idx_t temp = x;
        int omega = 0;
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                ++omega;
                while (temp % p == 0)
                    temp /= p;
            }
        }
        if (temp > 1)          /* remaining prime factor */
            ++omega;

        /* log(log(x)) with floor; for x=2 the value is negative */
        double v = log(log((double)x));
        int fl  = (int)floor(v);

        if (omega > fl)
            ++count;
    }

    return (seq_t)x;
}
