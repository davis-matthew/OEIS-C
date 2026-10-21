/*
 * A206449  Values of sigma(p) of primes p such that sigma(p) = sigma(c) has a
 *          solution for some composite number c.
 * https://oeis.org/A206449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 2630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A206449 – values of σ(p) of primes p such that σ(p) = σ(c)
   for some composite c.
   a(n) = A158913(n) + 1   (see comment in OEIS entry)   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static idx_t   current_limit = 0;
static seq_t *values = NULL;         /* array of a(n) */
static idx_t  values_len = 0;

/* Compute the sequence up to at least 'need' terms.
   If 'need' <= values_len, nothing is done. */
static void compute_up_to(idx_t need)
{
    if (need <= values_len)
        return;

    /* If we already have a limit, try to extend it; otherwise start.
       We use a simple doubling strategy. */
    if (current_limit == 0)
        current_limit = 2000000ULL;   /* initial limit */
    else
        current_limit = current_limit * 2U;

    /* Increase until we collect enough terms. */
    while (values_len < need) {
        /* Free old data if re‑computing with a larger limit. */
        free(values);
        values = NULL;
        values_len = 0;

        /* Allocate temporary arrays. */
        unsigned char *is_prime = (unsigned char *)malloc((current_limit + 1) * sizeof(unsigned char));
        if (!is_prime) return;

        /* Sieve of Eratosthenes. */
        for (idx_t i = 0; i <= current_limit; ++i)
            is_prime[i] = 1U;
        if (current_limit >= 0) is_prime[0] = 0U;
        if (current_limit >= 1) is_prime[1] = 0U;

        for (idx_t i = 2; (idx_t)i * i <= current_limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= current_limit; j += i)
                    is_prime[j] = 0U;
            }
        }

        /* Compute σ(n) for 1 <= n <= current_limit
           using a divisor summatory sieve. */
        uint64_t *sigma = (uint64_t *)malloc((current_limit + 1) * sizeof(uint64_t));
        if (!sigma) { free(is_prime); return; }
        for (idx_t i = 0; i <= current_limit; ++i)
            sigma[i] = 0ULL;

        for (idx_t d = 1; d <= current_limit; ++d) {
            for (idx_t m = d; m <= current_limit; m += d)
                sigma[m] += (uint64_t)d;
        }

        /* Mark which σ values are attained by composites. */
        unsigned char *reach = (unsigned char *)calloc((current_limit + 2), sizeof(unsigned char));
        if (!reach) { free(is_prime); free(sigma); return; }

        for (idx_t c = 4; c <= current_limit; ++c) {               /* c composite */
            if (!is_prime[c]) {
                uint64_t s = sigma[c];
                if (s <= current_limit + 1)                          /* fits in table */
                    reach[(idx_t)s] = 1U;
            }
        }

        /* Estimate capacity: worst case every prime qualifies. */
        idx_t prime_cnt = 0;
        for (idx_t p = 2; p <= current_limit; ++p)
            if (is_prime[p]) ++prime_cnt;

        seq_t *tmp = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
        if (!tmp) { free(is_prime); free(sigma); free(reach); return; }

        idx_t idx = 0;
        for (idx_t p = 2; p <= current_limit; ++p) {
            if (!is_prime[p])
                continue;
            uint64_t target = (uint64_t)p + 1ULL;
            if (target <= current_limit + 1 && reach[(idx_t)target]) {
                tmp[idx++] = (seq_t)target;
            }
        }
        free(is_prime);
        free(sigma);
        free(reach);

        /* Store the sequence. */
        values = tmp;
        values_len = idx;

        /* If not enough terms yet, double limit and try again. */
        if (values_len < need)
            current_limit = current_limit * 2U;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (!initialized) {
        compute_up_to(n);
        initialized = true;
    } else {
        compute_up_to(n);
    }

    if (n > values_len || !values)
        return 0;
    return values[n - 1];
}
