/*
 * A056582  Highest common factor (or GCD) of n^n and hyperfactorial(n-1),
 *          i.e., gcd(n^n, product(k^k) for k < n).
 * https://oeis.org/A056582
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = GCD(n^n, product(k^k) for k<n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

/* Integer exponentiation by squaring for seq_t.
   Works for built-in unsigned integers and for
   arbitrary‑precision seq_t types that provide the
   usual operators. */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is irrelevant */

    seq_t result = 1;

    /* Factor n: trial division up to sqrt(n). */
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* Count exponent of p in n. */
            idx_t e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }

            /* Exponent of p in n^n is e * n. */
            idx_t eMax = e * n;

            /* Exponent of p in H(n−1) = Σ_{i≥1} p^i * ⌊(n−1)/p^i⌋ * (⌊(n−1)/p^i⌋+1)/2. */
            idx_t S = 0;
            idx_t power = p;
            idx_t last = n - 1;
            while (power <= last) {
                idx_t cnt = last / power;
                /* Compute term = power * cnt * (cnt + 1) / 2 safely. */
                idx_t term = power * cnt * (cnt + 1) / 2;
                S += term;
                if (power > last / p) break; /* avoid overflow of power */
                power *= p;
            }

            idx_t exponent = eMax < S ? eMax : S;
            seq_t p_pow = pow_uint((uint64_t)p, exponent);
            result *= p_pow;
        }
    }

    /* Remaining prime factor, if any. */
    if (tmp > 1) {
        idx_t p = tmp;
        idx_t e = 1;
        idx_t eMax = e * n;

        idx_t S = 0;
        idx_t power = p;
        idx_t last = n - 1;
        while (power <= last) {
            idx_t cnt = last / power;
            idx_t term = power * cnt * (cnt + 1) / 2;
            S += term;
            if (power > last / p) break;
            power *= p;
        }

        idx_t exponent = eMax < S ? eMax : S;
        seq_t p_pow = pow_uint((uint64_t)p, exponent);
        result *= p_pow;
    }

    return result;
}
