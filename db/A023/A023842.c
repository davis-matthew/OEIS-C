/*
 * A023842  Sum of exponents in prime-power factorization of C(5n,n-5).
 * https://oeis.org/A023842
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime} ( v_p((5n)!) - v_p((n-5)!) - v_p((4n+5)!) )
 *            where v_p(m!) is the exponent of prime p in m!
 * Range:     u8 n <= 680, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9940 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* generate all primes ≤ limit via a simple sieve.
   The function allocates an array of primes which must be freed
   by the caller.  *cnt_out receives the number of primes returned. */
static uint64_t *simple_primes_upto(idx_t limit, size_t *cnt_out)
{
    if (limit < 2) { *cnt_out = 0; return NULL; }

    uint8_t *is_comp = (uint8_t*)calloc((size_t)(limit + 1), 1);
    if (!is_comp) { *cnt_out = 0; return NULL; }

    uint64_t *primes = NULL;
    size_t sz   = 0, cap = 0;

    for (idx_t num = 2; num <= limit; ++num) {
        if (!is_comp[(size_t)num]) {
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) { free(is_comp); free(primes); *cnt_out = 0; return NULL; }
                primes = tmp;
                cap = new_cap;
            }
            primes[sz++] = (uint64_t)num;

            if (num * num <= limit) {
                for (idx_t j = num * num; j <= limit; j += num)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    free(is_comp);
    *cnt_out = sz;
    return primes;
}

/* exponent of p in m! */
static seq_t v_p_factorial(idx_t m, uint64_t p)
{
    seq_t sum = 0;
    idx_t k = m;
    while (k) {
        k /= p;
        sum += k;
    }
    return sum;
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;

    idx_t limit = 5 * n;
    size_t prime_cnt;
    uint64_t *primes = simple_primes_upto(limit, &prime_cnt);
    if (!primes || prime_cnt == 0) return (seq_t)0;

    seq_t total = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t exp = v_p_factorial(5 * n, p)
                  - v_p_factorial(n - 5, p)
                  - v_p_factorial(4 * n + 5, p);
        total += exp;
    }

    free(primes);
    return total;
}
