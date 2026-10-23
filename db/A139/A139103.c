/*
 * A139103  Numbers that show the distribution of prime numbers up to the n-th
 *          prime using "0" for primes and "1" for nonprime numbers.
 * https://oeis.org/A139103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number whose k‑th digit (1 ≤ k ≤ pₙ) is * 0 if k
 *            is prime, 1 otherwise, where pₙ is the n‑th prime. * This
 *            implements the OEIS sequence A139103
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n < 1)
        return 0;

    /* Estimate an upper bound for the n‑th prime.  For n < 6 the
     * 6th prime is 13, so 15 is a safe limit for the few small cases.
     * For n ≥ 6 we use the classical bound pₙ < n (log n + log log n). */
    idx_t limit;
    if (n < 6) {
        limit = 15;                /* covers primes up to 11 */
    } else {
        double dn  = (double) n;
        double est = dn * (log(dn) + log(log(dn))) + 3.0;
        limit = (idx_t) est;
    }

    /* Allocate a boolean array for the sieve.  Char is used for minimal
     * memory, 0 means prime, 1 means composite. */
    size_t sz = (size_t)(limit + 1);
    char *is_comp = (char *)malloc(sz);
    if (!is_comp)
        return 0;                    /* allocation failure -> 0 */

    memset(is_comp, 0, sz);
    is_comp[0] = 1;
    is_comp[1] = 1;

    /* Simple Sieve of Eratosthenes.  Uses only integer arithmetic. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Find the n‑th prime pₙ. */
    idx_t primes_seen = 0;
    idx_t p_n = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            ++primes_seen;
            if (primes_seen == n) {
                p_n = i;
                break;
            }
        }
    }

    if (p_n == 0) {          /* should not happen for n ≥ 1 */
        free(is_comp);
        return 0;
    }

    /* Build the result by appending digits 0/1.  The result may be an
     * arbitrary‑precision integer; arithmetic operations work as with
     * built‑in integers. */
    seq_t result = 0;
    seq_t ten = (seq_t)10;

    for (idx_t k = 1; k <= p_n; ++k) {
        seq_t digit = is_comp[k] ? 1 : 0;
        result = result * ten + digit;
    }

    free(is_comp);
    return result;
}
