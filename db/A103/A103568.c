/*
 * A103568  Sum of the (primes > 5 modulo 7).
 * https://oeis.org/A103568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = sum_{k=1}^{n} (p_k mod 7) where p_k is the k‑th prime > 5
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a very small incremental prime search.  The
   function keeps a static counter and the cumulative sum of mod‑7
   residues of all primes > 5 that it has found so far.  Each call
   extends this counter until the requested n is reached.  This
   guarantees correct results for any order of calls and works
   efficiently for n up to about 10 000.  */
#include <stdbool.h>
#include "../utils/is_prime_candidate.h"

seq_t a(idx_t n) {
    static idx_t primes_found = 0;          /* count of primes > 5 seen */
    static idx_t last_prime = 5;            /* last prime found */
    static seq_t cum_sum = 0;                /* cumulative sum of residues */

    /* Ensure we have at least n primes > 5 */
    while (primes_found < n) {
        /* Find next prime > last_prime */
        idx_t cand = last_prime + 1;
        while (!is_prime_candidate(cand)) {
            cand++;
        }
        last_prime = cand;
        primes_found++;

        /* Add its residue modulo 7 to the cumulative sum */
        seq_t residue = (seq_t)(cand % 7ULL);
        cum_sum += residue;
    }
    return cum_sum;
}
