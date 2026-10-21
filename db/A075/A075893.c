/*
 * A075893  Average of three successive primes squared,
 *          (prime(n)^2+prime(n+1)^2+prime(n+2)^2)/3, n>=3.
 * https://oeis.org/A075893
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)^2 + prime(n+1)^2 + prime(n+2)^2)/3
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 4148, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the average of squares of three successive primes.
 * The implementation lazily computes the first N primes using
 * sieve_first_n_primes from the utilities.  It caches the primes
 * so that subsequent calls are cheap.  The function returns 0
 * for n < 3, which is outside the defined domain but harmless.
 */
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_list = NULL;   /* array of primes, 0‑indexed */
static size_t   prime_cnt   = 0;       /* number of primes stored */

/* Ensure at least 'needed' primes are available.  If we need more,
 * regenerate the list from scratch.  This keeps the implementation
 * simple while staying within the time limits for n up to ~10000. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt < (size_t)needed) {
        uint32_t *new_list = NULL;
        size_t   new_cnt   = 0;
        sieve_first_n_primes(needed, &new_list, &new_cnt);
        free(prime_list);
        prime_list = new_list;
        prime_cnt  = new_cnt;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined for n < 3, safe fallback */

    /* We need primes up to index n+2 (1‑based).  Convert to 0‑based. */
    size_t needed = (size_t)n + 2;
    ensure_primes(needed);

    /* Retrieve the three successive primes. */
    seq_t p1 = (seq_t)prime_list[(size_t)n - 1];     /* prime(n)   */
    seq_t p2 = (seq_t)prime_list[(size_t)n];         /* prime(n+1) */
    seq_t p3 = (seq_t)prime_list[(size_t)n + 1];     /* prime(n+2) */

    /* Compute the sum of squares and take the integer average. */
    seq_t sum = p1 * p1 + p2 * p2 + p3 * p3;
    return sum / (seq_t)3;
}
