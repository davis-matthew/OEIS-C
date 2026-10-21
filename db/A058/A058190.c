/*
 * A058190  Number of increasing geometric progressions ending in n (in the
 *          positive integers), excluding those of length 1 or 2.
 * https://oeis.org/A058190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{p≥2, p²≤n} φ(p) · (v_p(n) – 1) * where v_p(n) is the
 *            largest exponent e such that p^e divides n * and φ(p) is Euler’s
 *            totient function of p. * The sum runs over all integers p>1;
 *            composite p are allowed, * but p^2 must not exceed n so the
 *            exponent e≥2 is possible only * for p ≤ √n. Co...
 * Range:     u8 n <= 9927, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t if needed */

static seq_t phi_of(idx_t x)
{
    seq_t result = (seq_t)x;
    for (idx_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            result -= result / (seq_t)d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) {
        result -= result / (seq_t)x;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* iterate over all integers p with p² ≤ n */
    for (idx_t p = 2; p <= n / p; ++p) {
        idx_t temp = n;
        idx_t exp  = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp >= 2) {
            seq_t ph = phi_of(p);
            total += ph * (seq_t)(exp - 1u);
        }
    }
    return total;
}
