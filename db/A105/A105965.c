/*
 * A105965  Numbers n such that n = 2^i_1+2^i_2+...2^i_k =
 *          b(j_1)*b(j_2)*...b(j_k) for distinct i's and distinct j's, where b
 *          is A050376.
 * https://oeis.org/A105965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   popcnt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A105965
   a(n) = the nth integer >1 for which
           popcnt(n) = Σ popcnt(e_i)
        where n = Π p_i^e_i is the prime factorization of n.
   We generate the terms in increasing order, caching them for repeated
   calls.  The first 24 terms are
   2,4,6,10,12,16,18,20,33,34,36,42,48,56,65,68,70,80,84,88,104,120,129,138.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/popcnt_idx.h"

/* global state for cached terms */
static seq_t *terms = NULL;
static idx_t terms_size = 0;
static idx_t terms_cap  = 0;

/* state for sieve of smallest prime factors */
static unsigned int *spf = NULL;    /* spf[i] = smallest prime factor of i (i>=2) */
static idx_t sieve_limit = 0;       /* largest i for which spf is defined */

/* list of primes produced by the sieve */
static unsigned int *primes = NULL;
static idx_t primes_count = 0;
static idx_t primes_cap   = 0;

/* current candidate number to test */
static idx_t cur = 1;

/* ----------------------------------------------------------------------
   Grow the terms array to at least min_needed elements.
   ---------------------------------------------------------------------- */
static void grow_terms(idx_t min_needed)
{
    if ((size_t)terms_cap < (size_t)min_needed) {
        size_t new_cap = terms_cap ? (size_t)terms_cap * 2 : 4;
        while ((size_t)new_cap < (size_t)min_needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = (idx_t)new_cap;
    }
}

/* ----------------------------------------------------------------------
   Expand the spf sieve to cover numbers up to 'limit'.
   The function recomputes the sieve from scratch each time it is called,
   but we only call it when 'limit' is larger than the current sieve_limit.
   ---------------------------------------------------------------------- */
static void extend_sieve_to(idx_t limit)
{
    if (limit <= sieve_limit) return;

    size_t new_limit = (size_t)limit + 1;   /* array size for indices 0..limit */
    unsigned int *tmp_spf = (unsigned int*)realloc(spf, new_limit * sizeof(unsigned int));
    if (!tmp_spf) abort();
    spf = tmp_spf;
    for (size_t i = 0; i < new_limit; ++i) spf[i] = 0;

    /* simple sieve of Eratosthenes to mark primes and smallest prime factors */
    for (size_t i = 2; i < new_limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (unsigned int)i;
            /* store prime */
            if (primes_count == primes_cap) {
                size_t new_cap = primes_cap ? (size_t)primes_cap * 2 : 4;
                unsigned int *tmpP = (unsigned int*)realloc(primes, new_cap * sizeof(unsigned int));
                if (!tmpP) abort();
                primes = tmpP;
                primes_cap = (idx_t)new_cap;
            }
            primes[primes_count++] = (unsigned int)i;
            if (i * i < new_limit) {
                for (size_t j = i * i; j < new_limit; j += i) {
                    if (spf[j] == 0) spf[j] = (unsigned int)i;
                }
            }
        }
    }
    sieve_limit = limit;
}

/* ----------------------------------------------------------------------
   Return the n-th term of A105965 (offset 1).
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* caller must use n >= 1 */

    while (terms_size < n) {
        ++cur;                                 /* next candidate number */

        /* enlarge sieve only occasionally to keep time reasonable */
        if (cur > sieve_limit)          /* we need to sieve up to some lookahead */
            extend_sieve_to(cur * 2);

        unsigned int pop_n = popcnt_idx(cur);   /* popcount(n) */

        /* Sum of popcounts of the exponents in the prime factorization */
        unsigned int pop_exp_sum = 0;
        idx_t x = cur;
        while (x > 1) {
            unsigned int p = spf[(size_t)x];
            unsigned int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            pop_exp_sum += popcnt_idx(e);
        }

        if (pop_n == pop_exp_sum) {           /* satisfies the defining condition */
            grow_terms(terms_size + 1);
            terms[terms_size++] = (seq_t)cur;
        }
    }
    return terms[n - 1];
}
