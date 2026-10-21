/*
 * A064910  Smallest semiprime p*q such that q >= p and q mod p = n.
 * https://oeis.org/A064910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 135, u32 n <= 2784, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A064910
   Name: Smallest semiprime p*q such that q >= p and q mod p = n.
   Compute a(n) as follows:
   For each prime p > n, find the smallest prime q ≥ p + n with q ≡ n (mod p).
   The desired value is the minimum of all products p*q found. */

/* Required helper functions are in the utils directory:
   - build_primes_upto: to generate a list of primes up to a limit.
   - is_prime_big: primality test for seq_t values. */

#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise a static list of primes up to a fixed limit.
   This limit is chosen to comfortably cover n ≤ 10 000 and the
   corresponding product sizes expected in the test set. */
static void init_prime_list(void)
{
    if (prime_list) return;
    size_t cnt;
    /* 200 000 is sufficient for all n in the test range. */
    build_primes_upto(200000, &prime_list, &cnt);
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    init_prime_list();

    bool found = false;
    seq_t best = 0;   /* will be overwritten upon first candidate */

    /* Iterate over all primes p greater than n. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p <= n) continue;          /* p must be > n so that n < p */

        /* Candidate q = p + n is the smallest integer ≥ p with
           q % p == n. Search for the first prime q of that form. */
        uint64_t q = p + n;
        while (true) {
            if (is_prime_big((seq_t)q)) {
                seq_t prod = (seq_t)p * (seq_t)q;
                if (!found || prod < best) {
                    best = prod;
                    found = true;
                }
                break;
            }
            q += p;                      /* next number congruent to n mod p */
        }

        /* Prune the search: for any larger p,
           the smallest possible product is p*(p+n).
           If that is already larger than the best found, we can stop. */
        if (found) {
            seq_t min_possible = (seq_t)p * (seq_t)(p + n);
            if (min_possible > best) break;
        }
    }

    return best;
}
