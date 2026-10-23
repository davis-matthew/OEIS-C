/*
 * A180470  a(n) = -prime(n) + prime(n + prime(n)) - 1.
 * https://oeis.org/A180470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = -prime(n) + prime(n + prime(n)) - 1
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS sequence A180470.
 * The implementation incrementally builds the list of primes
 * using the provided is_prime() helper, which works for any
 * seq_t/idx_t implementation.  This avoids use of <math.h>
 * or floating‑point arithmetic.  The prime list is extended
 * only when required, keeping the total work well below the
 * time limits for n up to 10 000.  The function works in any
 * order of calls and preserves its state between calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static uint64_t *prime_cache = NULL;   /* list of primes (1‑based index)   */
static size_t   prime_cnt   = 0;        /* number of primes stored          */
static size_t   prime_cap   = 0;        /* capacity of prime_cache          */
static uint64_t next_candidate = 2;     /* next odd number to test         */

/* Ensure that prime_cache holds at least 'needed' primes. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;                      /* already have enough primes */

    if (prime_cache == NULL) {
        prime_cap = 64;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache)
            exit(EXIT_FAILURE);
    }

    while (prime_cnt < (size_t)needed) {
        uint64_t cand = next_candidate;

        /* Advance next_candidate for next round.  Skip evens. */
        next_candidate = cand + 1;
        if ((next_candidate & 1) == 0)
            next_candidate += 1;

        if (is_prime(cand)) {          /* cand is prime */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;       /* double capacity */
                prime_cache = (uint64_t *)realloc(prime_cache,
                                                  prime_cap * sizeof(uint64_t));
                if (!prime_cache)
                    exit(EXIT_FAILURE);
            }
            prime_cache[prime_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* below OEIS offset: ignore */

    ensure_primes(n);                   /* prime(n) */
    uint64_t p_n = prime_cache[n - 1];

    idx_t idx_next = n + (idx_t)p_n;    /* n + prime(n) */

    ensure_primes(idx_next);            /* prime(n + prime(n)) */
    uint64_t p_next = prime_cache[idx_next - 1];

    return (seq_t)p_next - (seq_t)p_n - 1;
}
