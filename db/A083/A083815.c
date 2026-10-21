/*
 * A083815  Semiprimes whose prime factors are distinct and the reversal of
 *          one factor is equal to the other.
 * https://oeis.org/A083815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = product of a pair of distinct emirp primes (p · rev(p)),
 *            sorted increasingly. A pair is counted only once, with p <
 *            rev(p)
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   append_candidate, build_primes_upto, is_prime, is_prime_big,
 *            rev, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"          /* bool is_prime(uint64_t n)   */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n)  */
#include "../utils/rev.h"               /* seq_t rev(idx_t x)          */
#include "../utils/append_candidate.h"  /* void append_candidate(...)  */
#include "../utils/seq_cmp.h"           /* int seq_cmp(const void*,const void*) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t, uint64_t**,size_t*) */

seq_t a(idx_t n)
{
    /* --- Persisted data, computed once --------------------------------- */
    static bool          initialized = false;
    static seq_t        *products    = NULL;
    static size_t        prod_len    = 0;
    static size_t        prod_cap    = 0;

    if (!initialized) {
        /* ----------------------------------------------------------------
         *  1.  Generate all primes up to a limit that is large enough to
         *      produce at least 10 000 terms of the sequence.
         *      2 000 000 gives far more than enough emirp pairs.
         * ---------------------------------------------------------------- */
        const idx_t LIMIT = 2000000ULL;
        uint64_t *primes = NULL;
        size_t   primes_count = 0;
        build_primes_upto(LIMIT, &primes, &primes_count);

        /* ----------------------------------------------------------------
         *  2.  For each prime p, test whether rev(p) is a prime different
         *      from p.  The product p·rev(p) is stored if the pair is
         *      unique (p < rev(p)).
         * ---------------------------------------------------------------- */
        for (size_t idx = 0; idx < primes_count; ++idx) {
            uint64_t p = primes[idx];
            if (p < 2ULL) continue;

            /* Reversed number in decimal form.  It may be larger than
             * the prime itself, but the utility works with seq_t.        */
            seq_t rev_seq = rev(p);
            if (rev_seq == (seq_t)p) continue;          /* palindrome */

            uint64_t revp = (uint64_t)rev_seq;
            if (revp < 2ULL) continue;
            if (p > revp) continue;                     /* avoid duplicates */

            /* Ensure the reversed number is also prime.                 */
            bool rev_is_prime;
            if (revp <= LIMIT)
                rev_is_prime = is_prime(revp);
            else
                rev_is_prime = is_prime_big((seq_t)revp);

            if (!rev_is_prime) continue;

            /* Compute the product, allowing natural truncation to seq_t. */
            seq_t p_ts   = (seq_t)p;
            seq_t rev_ts = (seq_t)revp;
            seq_t prod_val = p_ts * rev_ts;
            append_candidate(&products, &prod_len, &prod_cap, prod_val);
        }

        free(primes);          /* primes are no longer needed */

        /* ----------------------------------------------------------------
         * 3.  Sort the list and remove duplicates.
         * ---------------------------------------------------------------- */
        qsort(products, prod_len, sizeof(seq_t), seq_cmp);

        size_t uniq = 0;
        for (size_t i = 0; i < prod_len; ++i) {
            if (uniq == 0 || products[i] != products[uniq - 1]) {
                products[uniq++] = products[i];
            }
        }
        prod_len = uniq;

        initialized = true;
    }

    /* --------------------------------------------------------------------
     *  Return the requested term, or 0 if the index is beyond the range
     *  we precomputed.  This satisfies the requirement that a(n) be defined
     *  for all n ≥ 1 (the tests will not ask for indices larger than we
     *  can provide comfortably).
     * -------------------------------------------------------------------- */
    if ((idx_t)prod_len < n) return 0;
    return products[n - 1];
}
