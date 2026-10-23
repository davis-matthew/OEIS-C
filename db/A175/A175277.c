/*
 * A175277  Base-5 pandigital primes: primes having at least one of each digit
 *          0,1,2,3,4, when written in base 5.
 * https://oeis.org/A175277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime that is base‑5 pandigital * (contains each
 *            of the digits 0,1,2,3,4 at least once when written * in base 5).
 *            The implementation searches primes sequentially, * stores the
 *            ones that satisfy the pandigital test and returns the *
 *            requested element. It is memoised so that calls w...
 * Range:     u8 none, u16 n <= 727, u32 n <= 1015, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_base5_pandigital, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_base5_pandigital.h"

/* static storage for memoisation */
static seq_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static seq_t next_candidate = 3125ULL; /* 5^5 */

/* grow the primes array if needed */
static void ensure_capacity(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    while (need > new_cap)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1); /* allocation failure */
    primes = new_arr;
    primes_cap = new_cap;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* behaviour below offset does not matter */

    /* initialise (only on first call) */
    if (primes == NULL) {
        primes = NULL;
        primes_len = 0;
        primes_cap = 0;
        /* ensure odd start for all subsequent candidates */
        if ((next_candidate & 1) == 0)
            next_candidate++;
    }

    /* generate primes until we have at least n elements */
    while (primes_len < n) {
        /* find next prime that is base‑5 pandigital */
        while (true) {
            if (is_prime_big(next_candidate) && is_base5_pandigital(next_candidate)) {
                break; /* found one */
            }
            next_candidate += 2; /* skip even numbers */
        }

        /* store found prime */
        ensure_capacity(primes_len + 1);
        primes[primes_len++] = next_candidate;
        next_candidate += 2; /* advance for next search */
    }

    return primes[n - 1];
}
