/*
 * A056194  Characteristic cube divisor of n!: a(n) = A056191(n!).
 * https://oeis.org/A056194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n, v_p(n!) odd and >1} p³ where v_p(n!) is the
 *            exponent of prime p in the factorisation of n!. For example: n =
 *            4 → v₂(4!) = 3 (odd, >1) → include 2³ = 8 → a(4)=8 n = 6 →
 *            v₂(6!) = 4 (even) → exclude 2 v₃(6!) = 2 (even) → exclude 3 →
 *            a(6)=1 n = 22 → v₂(22!) = 19 (odd, >1) → 2³ v₃(22!)...
 * Range:     u8 n <= 14, u16 n <= 21, u32 n <= 38, u64 n <= 92, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) {
        seq_t one = 1;
        return one;
    }

    /* create a small sieve of Eratosthenes up to n */
    size_t limit = (size_t)n;
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) { /* allocation failed – return 0 (unlikely) */
        seq_t zero = 0;
        return zero;
    }
    for (size_t i = 2; i <= limit; ++i) {
        is_prime[i] = true;
    }

    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                is_prime[j] = false;
            }
        }
    }

    seq_t result = 1;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;

        /* compute exponent of p in n! using Legendre's formula */
        idx_t exp = 0;
        idx_t power = p;
        while (power <= n) {
            exp += n / power;
            if (power > n / p) break;  /* avoid overflow of power*=p */
            power *= p;
        }

        /* include p³ if exponent is odd and greater than 1 */
        if (exp > 1 && (exp & 1)) {
            seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
            result *= p3;
        }
    }

    free(is_prime);
    return result;
}
