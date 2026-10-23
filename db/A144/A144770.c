/*
 * A144770  Numbers with sum of divisors = 2880.
 * https://oeis.org/A144770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144770: numbers with sum of divisors σ(n) = 2880.
 * The function precomputes all n < 2880 satisfying σ(n)=2880
 * (there are finitely many) and returns the n‑th such number (1‑based). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Build a list of all primes up to 2879 once. */
static void build_prime_list(idx_t limit, idx_t **out, size_t *cnt_out)
{
    /* Allocate space for the primes; 2879 is small, so we can use a
       static array with a slightly larger bound. */
    static idx_t primes_static[400] = {0};
    static bool initialized = false;
    static size_t count = 0;

    if (!initialized) {
        /* Simple sieve up to limit. */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }
        count = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i]) primes_static[count++] = i;
        free(is_prime);
        initialized = true;
    }
    *out = primes_static;
    *cnt_out = count;
}

/* Compute σ(n) for n < 2880 using the precomputed prime list. */
static seq_t sigma_of(idx_t n)
{
    seq_t sigma = (seq_t)1;
    idx_t remaining = n;
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_prime_list(2879, &primes, &primes_cnt);

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= remaining; ++i) {
        idx_t p = primes[i];
        if (remaining % p == 0) {
            seq_t term_sum = (seq_t)1;
            seq_t p_power = (seq_t)1;
            while (remaining % p == 0) {
                remaining /= p;
                p_power *= (seq_t)p;
                term_sum += p_power;
            }
            sigma *= term_sum;
        }
    }
    if (remaining > 1) {
        sigma *= (seq_t)remaining + (seq_t)1;
    }
    return sigma;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t list[1024] = {0};
    static size_t count = 0;
    static bool ready = false;

    if (!ready) {
        for (idx_t x = 1; x < 2880; ++x) {
            if (sigma_of(x) == (seq_t)2880) {
                if (count < sizeof(list) / sizeof(list[0]))
                    list[count++] = (seq_t)x;
                /* the loop runs in increasing order, so list stays sorted */
            }
        }
        ready = true;
    }

    if (n == 0 || n > (idx_t)count)
        return (seq_t)0;      /* undefined for n == 0 or out of bounds */

    return list[(size_t)(n - 1)];
}
