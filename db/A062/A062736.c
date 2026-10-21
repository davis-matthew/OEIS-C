/*
 * A062736  Primes of form Sum_{k=1..n} (prime(k)+1).
 * https://oeis.org/A062736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2900, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062736: Primes of the form sum_{k=1..m} (prime(k)+1) = sum_{k=1..m} prime(k) + m */
/* Implementation uses a simple incremental prime search and checks each candidate S_m with isPrimeGeneric. */

#include <stdbool.h>                 /* for bool */
#include "../utils/isPrimeGeneric.h" /* isPrimeGeneric(seq_t n) */

seq_t a(idx_t n)
{
    /* Running sum of the first m primes, current prime index m (1‑based) */
    seq_t sum = 0;
    idx_t  m   = 0;
    /* How many S_m values we have found that are prime */
    idx_t  count = 0;
    /* Current candidate value S_m = sum + m */
    seq_t candidate = 0;

    /* Candidate prime for the ordinary prime sequence; start with 2 */
    seq_t prime_candidate = 2;

    while (count < n) {
        /* --- Find the next prime number --- */
        while (!isPrimeGeneric(prime_candidate)) {
            if (prime_candidate == 2)
                prime_candidate = 3;
            else
                prime_candidate += 2;
        }

        /* Update the sum and index */
        sum += prime_candidate;
        m++;

        /* Compute S_m and test for primality */
        candidate = sum + (seq_t)m;
        if (isPrimeGeneric(candidate))
            count++;

        /* Advance prime_candidate to the next odd number after the found prime */
        if (prime_candidate == 2)
            prime_candidate = 3;
        else
            prime_candidate += 2;
    }

    return candidate;
}
