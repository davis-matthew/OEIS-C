/*
 * A085976  Number of 2's in decimal expansion of prime(n).
 * https://oeis.org/A085976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = count of digit '2' in the decimal expansion of the n‑th
 *            prime (OEIS A085976). The nth prime is generated on demand by a
 *            simple sieve; the prime list is kept in a static array that is
 *            rebuilt only when a larger index is requested
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* ---------- static data used by the sieve ---------------------------------- */
static idx_t *prime_arr   = NULL;   /* array of primes */
static size_t prime_cnt   = 0;      /* number of primes stored */
static idx_t  sieve_limit = 0;      /* upper bound used for last sieve */

/* ---------- helper to rebuild the prime array ------------------------------ */
static void ensure_primes(idx_t n)
{
    /* If we already have n primes, nothing to do. */
    if (prime_cnt >= n) return;

    /* Estimate a sieve upper bound that certainly contains the n-th prime.
       For n ≤ 10000 the bound 15*n exceeds 104999 (the 10000th prime). */
    idx_t new_limit = (n < 6) ? 15 : (n * 15);

    /* If we already sieved to a larger limit, the existing primes cover n. */
    if (sieve_limit >= new_limit) {
        /* But we may still have prime_cnt < n if previous limit was wrong
           for very small n (when n < 6).  In that case we keep sieving. */
        if (prime_cnt >= n) return;
    }

    /* Allocate temporary sieve array. */
    unsigned char *sieve = (unsigned char *)malloc((size_t)(new_limit + 1));
    if (!sieve) return;           /* allocation failure – abort silently */

    /* Mark all numbers as potentially prime. */
    memset(sieve, 1, (size_t)(new_limit + 1));
    sieve[0] = sieve[1] = 0;

    /* Classic Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i) {
                sieve[j] = 0;
            }
        }
    }

    /* Reallocate prime array with a generous upper bound. */
    size_t max_estimate = new_limit / 10 + 10;   /* about 1/10 of numbers are prime */
    idx_t *new_arr = (idx_t *)malloc(max_estimate * sizeof(idx_t));
    if (!new_arr) {
        free(sieve);
        return;  /* allocation failure – abort silently */
    }

    /* Collect primes into the new array. */
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (sieve[i]) {
            if (pos >= max_estimate) break;   /* safety guard */
            new_arr[pos++] = i;
        }
    }

    /* Replace the old array. */
    free(prime_arr);
    prime_arr   = new_arr;
    prime_cnt   = pos;
    sieve_limit = new_limit;

    free(sieve);
}

/* ---------- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is 1‑based */

    /* Make sure we have at least the n‑th prime. */
    ensure_primes(n);

    /* Safety check – should never happen. */
    if (prime_cnt < n) return 0;

    idx_t p = prime_arr[n - 1];
    seq_t cnt = 0;

    /* Count digit '2' in decimal representation of p. */
    while (p > 0) {
        if ((p % 10) == 2) ++cnt;
        p /= 10;
    }
    return cnt;
}
