/*
 * A072971  Least k such that the last digit of prime(n+k) = last digit of
 *          prime(n) in base 10.
 * https://oeis.org/A072971
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k > 0 such that the last digit of prime(n+k)
 *            equals the last digit of prime(n). The function builds the list
 *            of primes on demand and searches for the required offset
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array of primes (1‑based indexing).  The first prime is stored
   at primes[1] = 2.  The array grows as needed. */
static uint64_t *primes = NULL;
static size_t prime_len = 0;     /* highest stored index (inclusive)   */
static size_t prime_cap = 0;     /* allocated capacity                 */
static uint64_t prime_next_candidate = 3;  /* next odd candidate after the last prime */

/* Ensure that the prime list contains at least p->idx entries.  */
static void
ensure_primes(idx_t need)
{
    if (prime_len == 0) {
        /* Initialisation: store the first prime 2. */
        prime_cap = 16;
        primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
        primes[1] = 2ULL;
        prime_len = 1;
        prime_next_candidate = 3ULL;
    }
    while (prime_len < need) {
        uint64_t cand = prime_next_candidate;
        bool is_prime = true;

        /* Trial division using already known primes. */
        for (size_t j = 1; j <= prime_len; ++j) {
            uint64_t p = primes[j];
            if (p > cand / p) break;   /* p^2 > cand */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_len + 1 >= prime_cap) {
                size_t new_cap = prime_cap * 2;
                primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                prime_cap = new_cap;
            }
            primes[++prime_len] = cand;
            prime_next_candidate = cand + 2ULL;   /* skip even numbers */
        } else {
            prime_next_candidate += 2ULL;
        }
    }
}

seq_t
a(idx_t n)
{
    /* For n < 4 the sequence is undefined; return 0. */
    if (n < 4) return (seq_t)0;

    /* Make sure prime[n] is available. */
    ensure_primes(n);

    uint64_t ld = primes[n] % 10U;  /* last digit of prime(n) */

    idx_t m = n + 1;
    for (;;) {
        ensure_primes(m);
        if ((primes[m] % 10U) == ld) {
            return (seq_t)(m - n);
        }
        ++m;
    }
}
