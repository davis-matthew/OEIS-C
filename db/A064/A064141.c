/*
 * A064141  Sum of non-unitary divisors of central binomial coefficient C(n,
 *          floor(n/2)).
 * https://oeis.org/A064141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of non-unitary divisors of C(n,⌊n/2⌋) A046098 numbers
 *            yield 0 because sum_all = sum_unitary
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   v_p_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/v_p_factorial.h"

/* Prime sieve – generates all primes ≤ limit.
   Uses a static vector for reuse across calls. */
static void generate_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    static bool init = false;
    static size_t max_limit = 0;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!init || limit > max_limit) {
        /* extend sieve */
        size_t new_limit = limit > max_limit * 2 ? limit : max_limit * 2;
        /* Simple incremental sieve */
        size_t *is_composite = NULL; /* usage omitted for brevity, assume simple sieve */
        /* For simplicity, use a naive method (acceptable for 10k). */
        idx_t *new_primes = NULL;
        size_t new_cnt = 0;
        for (idx_t i = 2; i <= new_limit; ++i) {
            bool composite = false;
            for (idx_t j = 0; j < new_cnt; ++j) {
                if (new_primes[j] * new_primes[j] > i) break;
                if (i % new_primes[j] == 0) { composite = true; break; }
            }
            if (!composite) {
                if (new_cnt == 0) new_primes = (idx_t*)malloc(sizeof(idx_t));
                else new_primes = (idx_t*)realloc(new_primes, (new_cnt+1)*sizeof(idx_t));
                new_primes[new_cnt++] = i;
            }
        }
        /* free old */
        free(primes);
        primes = new_primes;
        prime_cnt = new_cnt;
        max_limit = new_limit;
        init = true;
    }
    *primes_out = primes;
    *cnt_out = prime_cnt;
}

/* Fast exponentiation for seq_t.    */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used but safety */
    idx_t k = n / 2;               /* floor(n/2) */

    /* Generate primes up to n */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    generate_primes(n, &primes, &prime_cnt);

    seq_t sum_all = 1;
    seq_t sum_unitary = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;

        /* exponent of p in C(n,k) */
        idx_t exp = v_p_factorial(n, p)
                   - v_p_factorial(k, p)
                   - v_p_factorial(n - k, p);
        if (exp == 0) continue;

        seq_t p_seq = (seq_t)p;

        /* term for sum of all divisors: (p^(exp+1)-1)/(p-1) */
        seq_t term_div = (pow_seq(p_seq, exp + 1) - 1) / (p_seq - 1);
        sum_all *= term_div;

        /* term for unitary divisors: 1 + p^exp */
        seq_t term_unit = 1 + pow_seq(p_seq, exp);
        sum_unitary *= term_unit;
    }

    /* non‑unitary sum = all divisors sum – unitary divisors sum */
    return sum_all - sum_unitary;
}
