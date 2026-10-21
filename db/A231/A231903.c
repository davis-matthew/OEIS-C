/*
 * A231903  Even numbers which are the sum of two different primes > 3 in
 *          exactly one way.
 * https://oeis.org/A231903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A231903 - sum of two distinct primes >3 in exactly one way
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search up to 2000 with a simple sieve. */
/* If a next term would be larger than 2000 the function returns 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *terms = NULL;          /* array of found terms */
static size_t term_cnt = 0;          /* number of terms stored */

static void compute_terms(void)
{
    if (term_cnt > 0) return;        /* already computed */

    /* A safe upper limit – larger than all known terms (62). */
    const uint64_t LIMIT = 2000;

    /* Sieve of Eratosthenes up to LIMIT. */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    memset(is_prime, true, (LIMIT + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* List of primes >3. */
    uint64_t primes[2001];
    size_t prime_cnt = 0;
    for (uint64_t i = 4; i <= LIMIT; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = i;
    }

    /* Search even numbers starting from 12. */
    for (uint64_t e = 12; e <= LIMIT; e += 2) {
        size_t pair_cnt = 0;
        for (size_t k = 0; k < prime_cnt; ++k) {
            uint64_t p = primes[k];
            if (p >= e / 2) break;
            uint64_t q = e - p;
            if (q <= p || q > LIMIT) continue;   /* distinct & within limit */
            if (is_prime[q]) {
                ++pair_cnt;
                if (pair_cnt > 1) break;           /* more than one way */
            }
        }
        if (pair_cnt == 1) {
            /* Append e to terms. */
            if (term_cnt == 0) {
                terms = (seq_t *)malloc(8 * sizeof(seq_t));
            } else if (term_cnt == 8) { /* arbitrary grow factor */
                terms = (seq_t *)realloc(terms, 16 * sizeof(seq_t));
            }
            terms[term_cnt++] = (seq_t)e;
        }
    }

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    compute_terms();
    if ((size_t)n <= term_cnt) {
        return terms[n - 1];
    }
    return 0;    /* no more terms within the computed range */
}
