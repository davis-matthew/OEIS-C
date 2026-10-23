/*
 * A142787  Primes congruent to 13 mod 60.
 * https://oeis.org/A142787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 412, u32 n <= 413, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A142787 – primes congruent to 13 (mod 60)
    This function returns the n‑th prime p such that p % 60 == 13.
    It is computed by generating the sequence incrementally and
    caching the results so that subsequent calls in any order are fast.   */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *primes13 = NULL;
static size_t primes13_cap = 0;
static size_t primes13_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – value for n=0 is irrelevant */

    /* initialise cache with the first term */
    if (primes13_len == 0) {
        grow_seq(&primes13, &primes13_cap, 1);
        primes13[0] = (seq_t)13;
        primes13_len = 1;
    }

    /* generate further terms until we have at least n of them */
    uint64_t last = (uint64_t)primes13[primes13_len - 1];
    while (primes13_len < n) {
        last += 60;                      /* next candidate ≡ 13 (mod 60) */
        if (is_prime_big((seq_t)last)) {
            grow_seq(&primes13, &primes13_cap, primes13_len + 1);
            primes13[primes13_len] = (seq_t)last;
            ++primes13_len;
        }
    }

    return primes13[n - 1];
}
