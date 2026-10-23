/*
 * A189314  Nonprimes k such that 2*omega(4k) = omega(4k+2), where omega(j) is
 *          the number of distinct primes dividing j, A001221.
 * https://oeis.org/A189314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 3026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189314: Nonprimes k such that 2*omega(4k) == omega(4k+2) */
/* Implementation follows the definition verbatim, using a simple
   incremental search with memoisation. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

static seq_t *terms   = NULL;   /* stored terms of the sequence   */
static idx_t terms_cap = 0;     /* current allocation capacity    */
static idx_t terms_size = 0;    /* number of terms produced so far*/
static idx_t next_k = 1;        /* next candidate k to test       */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour for indices 0 is irrelevant */

    /* Generate terms until we have at least n of them */
    while (terms_size < n) {
        /* Iterate over successive candidates */
        for (;;) {
            if (!is_prime(next_k)) {
                uint64_t n4  = 4ULL * (uint64_t)next_k;
                uint64_t w1 = count_distinct_primes(n4);
                uint64_t w2 = count_distinct_primes(n4 + 2ULL);
                if ((2ULL * w1) == w2) {
                    /* Allocate more space if needed */
                    if (terms_size == terms_cap) {
                        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                        seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                        terms = tmp;
                        terms_cap = (idx_t)new_cap;
                    }
                    terms[terms_size++] = (seq_t)next_k;
                    /* Move to the next candidate for future calls */
                    next_k++;
                    break;
                }
            }
            /* Non‑qualifying candidate – try the next one */
            next_k++;
        }
    }
    return terms[n - 1];
}
