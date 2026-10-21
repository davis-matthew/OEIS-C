/*
 * A088388  Exponent of the largest prime power factor of n, a(1)=0.
 * https://oeis.org/A088388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = exponent of the largest prime‐power divisor of n (the
 *            prime power divisor with the greatest numeric value)
 * Range:     u8 n <= 9634, u16 n <= 9634, u32 n <= 9634, u64 n <= 9634,
 *            u128 n <= 9634, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)0;      /* a(1) = 0 by definition */

    idx_t m = n;
    seq_t max_val = (seq_t)1;          /* numeric value of the largest prime power seen */
    seq_t max_exp = (seq_t)0;          /* its exponent */

    /* factor 2 separately */
    if ((m & 1u) == 0u) {
        seq_t exp = (seq_t)0;
        seq_t val = (seq_t)1;
        while ((m & 1u) == 0u) {
            m >>= 1u;
            exp++;
            val <<= 1u;                /* val = 2^exp */
        }
        if (val > max_val) {
            max_val = val;
            max_exp = exp;
        }
    }

    /* odd factors */
    for (idx_t p = 3u; p * p <= m; p += 2u) {
        if (m % p == 0u) {
            seq_t exp = (seq_t)0;
            seq_t val = (seq_t)1;
            while (m % p == 0u) {
                m /= p;
                exp++;
                val *= (seq_t)p;        /* val = p^exp */
            }
            if (val > max_val) {
                max_val = val;
                max_exp = exp;
            }
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (m > 1u) {
        seq_t exp = (seq_t)1;
        seq_t val = (seq_t)m;          /* val = m^1 */
        if (val > max_val) {
            max_val = val;
            max_exp = exp;
        }
    }

    return max_exp;
}
