/*
 * A025529  a(n) = (1/1 + 1/2 + ... + 1/n)*lcm{1,2,...,n}.
 * https://oeis.org/A025529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} lcm(1..n)/k (OEIS A025529)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* sequence start is n = 1 */

    /* ----- Prime sieve up to n ----- */
    bool *is_prime = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (is_prime == NULL) return 0;          /* allocation failed */

    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* ----- Compute L = lcm(1..n) ----- */
    seq_t L = 1;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p]) continue;

        /* maximal exponent e with p^e <= n */
        seq_t p_power = (seq_t)p;
        while (p_power <= (seq_t)(n / p)) {
            p_power *= (seq_t)p;
        }

        L *= p_power;
    }

    free(is_prime);

    /* ----- Sum L/k for k = 1..n ----- */
    seq_t ans = 0;
    for (idx_t k = 1; k <= n; ++k) {
        ans += L / (seq_t)k;
    }

    return ans;
}
