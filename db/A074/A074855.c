/*
 * A074855  Primes p such that all p-t for p/2 < t < p are nonprime, where t
 *          is a triangular number.
 * https://oeis.org/A074855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = the n‑th prime p such that for all triangular numbers t
 *            with p/2 < t < p the value p−t is composite or 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

/* Memoised sequence data */
static seq_t *seq = NULL;        /* array of terms */
static size_t seq_sz = 0;        /* number of terms stored */
static size_t seq_cap = 0;       /* allocated capacity */
static idx_t next_candidate = 2; /* integer to test for primality */

/* Return the n‑th term (n ≥ 1) of sequence A074855.  */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (seq_sz < n) {
        /* Find the next prime satisfying the property. */
        for (idx_t cand = next_candidate; ; ++cand) {
            next_candidate = cand + 1;            /* advance global counter */

            if (!isPrimeGeneric((seq_t)cand))
                continue;                         /* skip non‑prime candidates */

            /* For small primes 2 and 3 the property holds vacuously. */
            seq_t half = (seq_t)(cand / 2);
            bool good = true;
            idx_t  t  = 0;

            /* Enumerate triangular numbers T(k)=k(k+1)/2.  */
            for (idx_t k = 1; ; ++k) {
                t += k;                            /* t = T(k)   */
                if (t >= cand)
                    break;

                if (t > cand / 2) {                /* p/2 < t < p  */
                    seq_t d = (seq_t)(cand - t);
                    if (isPrimeGeneric(d)) {
                        good = false;             /* property violated */
                        break;
                    }
                }
            }

            if (!good)
                continue;                         /* test next candidate */

            /* Store the found prime as the next term. */
            if (seq_sz == seq_cap) {
                ensure_capacity(seq_sz + 1, &seq_cap,
                                (void **)&seq, sizeof(seq_t));
            }
            seq[seq_sz++] = (seq_t)cand;
            break;                                /* proceed to next needed term */
        }
    }
    return seq[n - 1];                               /* sequence is 1‑indexed */
}
