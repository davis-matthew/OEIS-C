/*
 * A083659  Denominator of fraction equal to the continued fraction [p(n);
 *          p(n-1),...,5,3,2].
 * https://oeis.org/A083659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of the continued fraction
 *            [p(n);p(n‑1),…,p(1)], where p(k) is the k‑th prime
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime sieve limit – enough for n up to at least 200 000. */
#define SIEVE_LIMIT 2000000

static bool sieve_done = false;
static unsigned int *prime_buf = NULL;   /* array of primes, ascending order */
static size_t prime_cnt = 0;             /* number of primes stored */

/* Perform a one‑time sieve of Eratosthenes up to SIEVE_LIMIT. */
static void init_primes(void) {
    if (sieve_done) return;
    sieve_done = true;

    /* Allocate a boolean array for the sieve. */
    bool *is_prime = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime) return;                /* allocation failure → give up */

    for (unsigned int i = 2; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;

    for (unsigned int i = 2; (unsigned long long)i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (unsigned int j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes to allocate buffer. */
    size_t cnt = 0;
    for (unsigned int i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_buf = (unsigned int *)malloc(cnt * sizeof(unsigned int));
    if (!prime_buf) {
        free(is_prime);
        return; /* give up */
    }

    size_t pos = 0;
    for (unsigned int i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_buf[pos++] = i;
    prime_cnt = cnt;
    free(is_prime);
}

/*
 * Compute the n‑th term of A083659.
 * n >= 1. Behaviour for n < 1 is undefined.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined; guard against accidental use */

    init_primes();
    if (prime_cnt < (size_t)n) return 0; /* n too large for the fixed sieve */

    /* Iteratively evaluate the fraction from right to left. */
    seq_t num = (seq_t)prime_buf[0];   /* start with the smallest prime (2) */
    seq_t den = 1;                    /* denominator of the right‑most term */

    for (idx_t i = 1; i < n; ++i) {
        seq_t a_i = (seq_t)prime_buf[i];
        seq_t new_num = a_i * num + den;
        seq_t new_den = num;
        num = new_num;
        den = new_den;
    }

    return den;   /* denominator of the whole continued fraction */
}
