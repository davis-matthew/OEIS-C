/*
 * A166566  Primes p such that product of digits - 1 is prime.
 * https://oeis.org/A166566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime p such that digit_product(p)−1 is prime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_product.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;   /* cache of found terms             */
    static idx_t len    = 0;      /* number of terms stored           */
    static size_t cap   = 0;      /* allocated capacity of seq        */
    static idx_t next_candidate = 3; /* candidate primes to test (start at 3) */

    if (n == 0) return 0;          /* offset is 1; 0 is a harmless value */

    /* Return cached value if already computed. */
    if (n <= len) return seq[n - 1];

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        /* Search for the next prime with the required property. */
        while (true) {
            idx_t cand = next_candidate++;          /* candidate integer */

            if (!is_prime_big((seq_t)cand)) continue;   /* must be prime */

            seq_t prod = digit_product((seq_t)cand);    /* product of digits */
            if (prod == (seq_t)0 || prod == (seq_t)1) continue;

            seq_t val = prod - (seq_t)1;                /* prod-1           */
            if (!is_prime_big(val)) continue;           /* val must be prime */

            /* Grow cache if necessary. */
            if (len + 1 > cap) {
                size_t newcap = cap ? cap * 2 : 32;
                seq_t *tmp = seq_realloc(seq, cap, newcap);
                if (!tmp) abort();   /* out of memory */
                seq = tmp;
                cap = newcap;
            }

            seq[len++] = (seq_t)cand;          /* store the found prime */
            break;                             /* move on to next term   */
        }
    }

    return seq[n - 1];
}
