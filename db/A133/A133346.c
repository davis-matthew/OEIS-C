/*
 * A133346  a(n) = smallest k such that prime(n+2) = prime(n) + (prime(n) mod
 *          k), or 0 if no such k exists.
 * https://oeis.org/A133346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that prime(n+2) = prime(n) + (prime(n)
 *            mod k), or 0 if no such k exists
 * Range:     u8 n <= 56, u16 n <= 6545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Prime cache ------------------------------------------------------- */

static uint64_t *primes = NULL;   /* array of the first primes found      */
static size_t   prime_cnt = 0;    /* current number of cached primes      */

/* add the next prime to the cache, used repeatedly to grow the list      */
static void add_next_prime(void)
{
    uint64_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
    for (;; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt &&
                            (uint64_t)i * (uint64_t)i <= cand; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) break;
    }
    uint64_t *tmp = (uint64_t *)realloc(primes, sizeof(uint64_t) * (prime_cnt + 1));
    if (tmp == NULL) abort();          /* if allocation fails, terminate */
    primes = tmp;
    primes[prime_cnt++] = cand;
}

/* ensure that at least `need` primes are cached                         */
static void ensure_primes(idx_t need)
{
    while (prime_cnt < (size_t)need) add_next_prime();
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, guard for n==0 */

    /* we need prime(n) and prime(n+2) -> at least n+2 primes            */
    ensure_primes(n + 2);

    uint64_t p  = primes[n - 1];          /* prime(n)  (1‑based)          */
    uint64_t p2 = primes[n + 1];          /* prime(n+2)                    */
    uint64_t diff = p2 - p;               /* prime(n+2) - prime(n)         */

    /* diff must be a valid remainder: 0 < diff < p                       */
    if (diff == 0 || diff > p) return 0;

    /* We need a divisor k of (p - diff) that is larger than diff.       */
    uint64_t d   = p - diff;              /* p - diff must be divisible by k*/
    if (d == 0) return 0;                 /* should not happen                 */

    seq_t best = 0;
    for (uint64_t i = 1; i * i <= d; ++i) {
        if (d % i == 0) {
            uint64_t k1 = i;
            uint64_t k2 = d / i;

            if (k1 > diff && (best == 0 || k1 < best)) best = (seq_t)k1;
            if (k2 > diff && k2 != k1 && (best == 0 || k2 < best)) best = (seq_t)k2;
        }
    }

    /* If no divisor larger than diff was found, d itself is the only
       possible candidate.                                                        */
    if (best == 0 && d > diff) best = (seq_t)d;

    return best;
}
