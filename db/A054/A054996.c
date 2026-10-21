/*
 * A054996  Integers that can be expressed as the sum of consecutive primes in
 *          exactly 1 way.
 * https://oeis.org/A054996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates all terms by counting representations as sums of consecutive primes
   in exactly one way.  The implementation builds the sequence on demand by
   repeatedly doubling the search limit until enough terms are found. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*--- Global storage for the sequence ----------------------------------*/
static seq_t *terms = NULL;        /* array of computed terms */
static size_t terms_len = 0;       /* number of terms stored            */
static size_t terms_cap = 0;       /* allocated capacity                 */

/*--- Ensure storage for terms -------------------------------------------*/
static void ensure_terms_cap(size_t cap) {
    if (terms_cap >= cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < cap) new_cap = cap;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/*--- Generate a list of primes up to 'limit' (inclusive) ----------------*/
static void sieve_primes(int limit, int **pp, size_t *pc) {
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; (long long)i * i <= limit; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    size_t cap = limit / 2 + 10;
    int *pr = (int *)malloc(cap * sizeof(int));
    size_t len = 0;
    for (int i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (len == cap) {
                cap *= 2;
                pr = (int *)realloc(pr, cap * sizeof(int));
            }
            pr[len++] = i;
        }
    }
    free(is_prime);
    *pp = pr;
    *pc = len;
}

/*--- Build the sequence up to the given search limit -------------------*/
static void build_sequence(int limit) {
    int *primes = NULL;
    size_t primes_len = 0;
    sieve_primes(limit, &primes, &primes_len);

    uint32_t *cnt = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    for (size_t i = 0; i < primes_len; ++i) {
        int sum = 0;
        for (size_t j = i; j < primes_len; ++j) {
            sum += primes[j];
            if (sum > limit) break;
            cnt[sum] += 1;
        }
    }

    ensure_terms_cap((size_t)(limit / 10 + 32));
    terms_len = 0;
    for (int s = 2; s <= limit; ++s) {
        if (cnt[s] == 1) {
            if (terms_len == terms_cap) ensure_terms_cap(terms_cap * 2);
            terms[terms_len++] = (seq_t)s;
        }
    }

    free(primes);
    free(cnt);
}

/*--- Public sequence function -------------------------------------------*/
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Grow the search limit until we have at least n terms.  Start with a
       modest limit and double it each time. */
    int limit = 1024;
    while (terms_len < (size_t)n) {
        build_sequence(limit);
        if (terms_len >= (size_t)n) break;
        limit *= 2;
    }
    return terms[n - 1];
}
