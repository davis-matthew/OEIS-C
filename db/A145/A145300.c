/*
 * A145300  a(n) is the maximal prime such that if p_n is the n-th prime then
 *          ceiling(sqrt(a(n)*p_n))^2 - a(n)*p_n is a perfect square.
 * https://oeis.org/A145300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt, get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145300: a(n) is the maximal prime q such that
 * ceil(sqrt(q * p_n))^2 - q * p_n is a perfect square,
 * where p_n is the n‑th prime.  This implementation
 * computes the value by iterating over primes between
 * p_n and the theoretical upper bound p_n + 2*sqrt(2*p_n)+2. */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* p_n: the n‑th prime (1‑based index) */
    seq_t p = (seq_t)get_prime(n);

    /* Upper bound given by the theorem in the OEIS entry */
    seq_t bound = p + 2 * floor_sqrt((seq_t)(2 * p)) + 2;

    /* best prime found so far (at least p itself satisfies) */
    seq_t best = p;

    /* iterate over primes q >= p and <= bound */
    uint64_t q = (uint64_t)p;          /* current candidate prime (uint64_t) */
    while (1) {
        /* m = q * p */
        seq_t m = (seq_t)q * p;

        /* ceil(sqrt(m)) */
        seq_t r = floor_sqrt(m);
        seq_t t = (r * r == m) ? r : r + 1;

        /* k = t^2 - m and test if k is a perfect square */
        seq_t k = t * t - m;
        seq_t s = floor_sqrt(k);
        if (s * s == k) {
            best = (seq_t)q;
        }

        /* advance to the next prime */
        q = next_prime(q + 1);
        if ((seq_t)q > bound) {
            break;
        }
    }

    return best;
}
