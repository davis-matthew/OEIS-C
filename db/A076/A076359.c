/*
 * A076359  a(n) = denominator(n!/phi(n!)).
 * https://oeis.org/A076359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator(n!/phi(n!))
 * Range:     u8 n <= 16, u16 n <= 22, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   prime_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/prime_index.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* -------- sieve primes up to n -------- */
    bool *is_prime = (bool*)malloc((size_t)(n + 1) * sizeof(bool));
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) is_prime[j] = false;
        }
    }

    int *prime_list = (int*)malloc((size_t)(n + 1) * sizeof(int));
    idx_t prime_count = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) prime_list[prime_count++] = (int)i;
    }
    free(is_prime);

    /* -------- exponent counts for denominator after cancellation -------- */
    int *exp = (int*)calloc((size_t)prime_count, sizeof(int));

    for (idx_t idx = 0; idx < prime_count; ++idx) {
        idx_t q = (idx_t)prime_list[idx];
        idx_t m = q - 1;

        /* factor m using primes up to sqrt(m) */
        for (idx_t f = 0; f < prime_count && (idx_t)prime_list[f] * (idx_t)prime_list[f] <= m; ++f) {
            int p = prime_list[f];
            int cnt = 0;
            while (m % (idx_t)p == 0) {
                m /= (idx_t)p;
                ++cnt;
            }
            if (cnt > 0) {
                idx_t pi = prime_index(prime_list, prime_count, p);
                exp[pi] += cnt;
            }
        }
        if (m > 1) {
            int mp = (int)m;                     /* m is prime and ≤ n-1 */
            idx_t pi = prime_index(prime_list, prime_count, mp);
            exp[pi] += 1;
        }

        /* subtract numerator factor q */
        exp[idx] -= 1;
    }

    /* -------- compute final denominator product -------- */
    seq_t result = 1;
    for (idx_t i = 0; i < prime_count; ++i) {
        int e = exp[i];
        if (e > 0) {
            seq_t base = (seq_t)prime_list[i];
            seq_t term = 1;
            int exp_local = e;
            while (exp_local > 0) {
                if (exp_local & 1) term *= base;
                exp_local >>= 1;
                if (exp_local > 0) base *= base;
            }
            result *= term;
        }
    }

    free(prime_list);
    free(exp);
    return result;
}
