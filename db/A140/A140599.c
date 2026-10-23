/*
 * A140599  Grand total of prime factors in composite runs between two primes:
 *          first composite.
 * https://oeis.org/A140599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   compute A140599 by scanning prime runs and testing prime sums of
 *            distinct prime factors For each pair of consecutive primes p and
 *            q, sum distinct prime factors of integers (p+1)..(q-1). If the
 *            sum is prime, store first composite (p+1) as a term
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static idx_t _seq_limit = 0;
static size_t _seq_terms_count = 0;
static seq_t * _seq_terms = NULL;

/* Rebuild the entire sequence up to the given limit.  */
static void rebuild_terms(idx_t new_limit)
{
    /* Free previous term buffer */
    free(_seq_terms);

    /* Sieve of Eratosthenes to obtain smallest prime factor array */
    uint64_t *spf = (uint64_t*)malloc((size_t)(new_limit + 1) * sizeof(uint64_t));
    if (!spf) return;
    for (size_t i = 0; i <= new_limit; ++i) spf[i] = 0;

    /* Array of all primes up to new_limit */
    idx_t *primes = (idx_t*)malloc((size_t)(new_limit + 1) * sizeof(idx_t));
    if (!primes) { free(spf); return; }
    size_t primes_cnt = 0;

    /* Linear sieve */
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            if (p > spf[i] || i * p > new_limit) break;
            spf[i * p] = p;
        }
    }

    /* Allocate term buffer (over-allocate for safety) */
    size_t terms_cap = (size_t)(new_limit / 2 + 10);
    seq_t *terms      = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) { free(spf); free(primes); return; }
    size_t terms_cnt = 0;

    /* Process every pair of consecutive primes */
    for (size_t idx = 0; idx + 1 < primes_cnt; ++idx) {
        uint64_t p1 = primes[idx];
        uint64_t p2 = primes[idx + 1];
        if (p2 <= p1 + 1) continue;

        seq_t sumDistinct = 0;
        for (uint64_t num = p1 + 1; num <= p2 - 1; ++num) {
            uint64_t x = num;
            uint64_t prev = 0;
            while (x > 1) {
                uint64_t d = spf[x];
                if (d != prev) {
                    sumDistinct += (seq_t)d;
                    prev = d;
                }
                while (x % d == 0) x /= d;
            }
        }

        if (is_prime_big(sumDistinct)) {
            if (terms_cnt == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) { free(spf); free(primes); free(terms); return; }
                terms = tmp;
            }
            terms[terms_cnt++] = (seq_t)(p1 + 1);
        }
    }

    /* Store computed data */
    free(spf);
    free(primes);
    _seq_limit        = new_limit;
    _seq_terms_count  = terms_cnt;
    _seq_terms        = terms;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (_seq_terms_count < (size_t)n) {
        idx_t new_limit = _seq_limit ? _seq_limit * 2 : 2000;
        rebuild_terms(new_limit);
    }
    return _seq_terms[n - 1];
}
