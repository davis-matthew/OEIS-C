/*
 * A096454  Primes p such that the number of primes q, 7 <= q < p, congruent
 *          to 1 or 2 mod 5, is one more than the number of such primes
 *          congruent to 3 or 4 mod 5.
 * https://oeis.org/A096454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that among all primes q with 7 ≤ q <
 *            p, the number of q ≡ 1 or 2 (mod 5) is one more than the number
 *            of such q ≡ 3 or 4 (mod 5)
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
 
seq_t a(idx_t n) {
    /* Static state for incremental generation */
    static seq_t *seq      = NULL;     /* cache of sequence values   */
    static idx_t seq_len   = 0;        /* number of cached values    */
    static idx_t seq_cap   = 0;        /* allocated capacity         */
    static idx_t *primes   = NULL;     /* list of already found primes */
    static idx_t primes_len  = 0;      /* count of stored primes     */
    static idx_t primes_cap  = 0;      /* allocated capacity of primes */
    static idx_t candidate   = 2;      /* next integer to test      */
    static idx_t cum1 = 0, cum2 = 0;   /* cumulative counts of
                                         * residues 1/2 and 3/4 */
    static bool initialized = false;
 
    /* Initialise static containers on first call */
    if (!initialized) {
        initialized = true;
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        primes_cap = 16;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    }
 
    /* Generate more terms until we have at least n */
    while (seq_len < n) {
        /* Find next prime */
        while (true) {
            /* Skip even numbers above 2 */
            if (candidate > 2 && (candidate & 1U) == 0U) {
                candidate++;
                continue;
            }
            /* Test primality against known primes */
            idx_t i;
            bool isprime = true;
            for (i = 0; i < primes_len; ++i) {
                idx_t p = primes[i];
                if (p > candidate / p)  /* p*p > candidate, no more division needed */
                    break;
                if (candidate % p == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime)
                break;       /* candidate is prime */
            candidate++;      /* test next number */
        }
 
        idx_t p = candidate;          /* new prime found */
        if (p >= 7) {
            /* Check the condition before including p itself */
            if (cum1 == cum2 + 1) {
                /* Ensure enough space */
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)p;   /* store the term */
            }
            /* Update cumulative counts based on p (it will be < next prime) */
            idx_t mod5 = p % 5;
            if (mod5 == 1 || mod5 == 2)
                cum1++;
            else  /* mod5 == 3 or 4 */
                cum2++;
        }
        /* Add p to prime list for future divisibility tests */
        if (primes_len == primes_cap) {
            primes_cap *= 2;
            primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
        }
        primes[primes_len++] = p;
 
        /* Prepare for next candidate */
        candidate++;
    }
 
    return seq[n - 1];
}
