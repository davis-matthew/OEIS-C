/*
 * A088196  Largest number that is not a quadratic residue modulo prime(n).
 * https://oeis.org/A088196
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest integer k < p_n such that (k/p_n) = –1, * where
 *            p_n is the n‑th prime (offset 2)
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Computes the n‑th prime if necessary and returns the largest
 * quadratic non‑residue modulo that prime. */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1) r = (__uint128_t)r * a % m;
        a = (__uint128_t)a * a % m;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0; /* out of defined range, return 0 */

    /* ----- lazily build a table of primes up to the required n ----- */
    static idx_t *primes = NULL;        /* array of primes 1‑indexed     */
    static size_t prime_cnt = 0;        /* number of primes stored       */
    static idx_t prime_limit = 0;       /* highest number sieved         */

    if (prime_cnt < n) {
        /* Rough upper bound for the n‑th prime: 20 * n + 100 works for n≤10000 */
        idx_t limit = n * 20 + 100;
        if (limit < prime_limit) limit = prime_limit;
        /* simple sieve of Eratosthenes */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t m = p * p; m <= limit; m += p) is_prime[m] = false;
            }
        }
        /* collect primes into a dynamic array */
        size_t cap = limit / 10 + 10;
        idx_t *new_p = (idx_t *)malloc(cap * sizeof(idx_t));
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                if (cnt >= cap) {
                    cap *= 2;
                    new_p = (idx_t *)realloc(new_p, cap * sizeof(idx_t));
                }
                new_p[cnt++] = i;
            }
        }
        free(is_prime);
        free(primes);
        primes = new_p;
        prime_cnt = cnt;
        prime_limit = limit;
    }

    idx_t prime = primes[n - 1];           /* p_n  */
    uint64_t exp = (prime - 1) / 2;        /* (p-1)/2  */

    /* search downward for the first non‑residue */
    for (idx_t k = prime - 1; k > 0; --k) {
        uint64_t r = modpow64((uint64_t)k, exp, (uint64_t)prime);
        if (r == prime - 1) {
            return (seq_t)k;
        }
    }
    return (seq_t)1; /* fallback – should never happen for odd primes */
}
