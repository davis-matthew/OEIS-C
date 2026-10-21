/*
 * A063696  Positions of positive coefficients in cyclotomic polynomial
 *          Phi_n(x), converted from binary to decimal.
 * https://oeis.org/A063696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binary number whose 1‑bits are exactly the exponents k
 *            for which the coefficient of x^k in the cyclotomic polynomial
 *            Φ_n(x) is +1 (constant term counted). a(0) = 0. The function
 *            computes Φ_n(x) by the Möbius factorisation Φ_n(x) = ∏_{d|n,
 *            μ(d)≠0} (x^{n/d}-1)^{μ(d)} and extracts the positiv...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    /* 1. factor n, collect distinct prime divisors */
    idx_t orig = n;
    idx_t tmp = n;
    idx_t primes[16];
    size_t pc = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pc++] = p;
            while (tmp % p == 0) {
                tmp /= p;
            }
        }
    }
    if (tmp > 1) {
        primes[pc++] = tmp;
    }

    /* 2. compute Euler's totient φ(n) */
    idx_t phi = n;
    tmp = n;
    for (size_t i = 0; i < pc; ++i) {
        idx_t p = primes[i];
        if (tmp % p == 0) {
            phi -= phi / p;
            while (tmp % p == 0) {
                tmp /= p;
            }
        }
    }

    /* 3. allocate coefficient arrays */
    size_t sz = (size_t)(phi + 1);
    int32_t *a_cur = (int32_t*)malloc(sz * sizeof(int32_t));
    int32_t *a_nxt = (int32_t*)malloc(sz * sizeof(int32_t));
    if (!a_cur || !a_nxt) {
        exit(1);  /* allocation failure */
    }
    for (idx_t i = 0; i <= phi; ++i) a_cur[i] = 0;
    a_cur[0] = 1;  /* start with Φ_1(x) = 1 */

    /* 4. enumerate all square–free divisors d of n */
    size_t subsets = (size_t)1 << pc;
    for (size_t mask = 0; mask < subsets; ++mask) {
        idx_t d = 1;
        size_t bits = 0;
        for (size_t j = 0; j < pc; ++j) {
            if (mask & ((size_t)1 << j)) {
                d *= primes[j];
                ++bits;
            }
        }
        idx_t m = orig / d;          /* exponent in the factor */
        int sign = (bits & 1) == 0 ? 1 : -1;  /* μ(d) */

        /* initialise the temporary array */
        for (idx_t i = 0; i <= phi; ++i) a_nxt[i] = 0;

        if (sign == 1) {  /* multiply by (x^m - 1) */
            for (idx_t i = 0; i <= phi; ++i) {
                int32_t val = a_cur[i];
                if (!val) continue;
                a_nxt[i] -= val;                /* coefficient of 1  */
                if (i + m <= phi) a_nxt[i + m] += val;  /* coefficient of x^m */
            }
        } else {  /* multiply by (x^m - 1)^(-1) = - Σ_{k>=0} x^{k·m} */
            for (idx_t i = 0; i <= phi; ++i) {
                int32_t val = a_cur[i];
                if (!val) continue;
                for (idx_t j = i; j <= phi; j += m) {
                    a_nxt[j] -= val;   /* coefficient of x^{j-i} is -1 */
                }
            }
        }

        /* swap current and next arrays */
        int32_t *tmp = a_cur;
        a_cur = a_nxt;
        a_nxt = tmp;
    }

    /* 5. build the mask from positive coefficients */
    seq_t mask = (seq_t)0;
    for (idx_t k = 0; k <= phi; ++k) {
        if (a_cur[k] > 0) {
            mask |= ((seq_t)1 << k);
        }
    }

    /* cleanup */
    free(a_cur);
    free(a_nxt);

    return mask;
}
