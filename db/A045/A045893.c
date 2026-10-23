/*
 * A045893  Solutions t to the equation s*prime(n) + t*prime(n+1) = 1 with |s|
 *          as small as possible.
 * https://oeis.org/A045893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = t where s*prime(n) + t*prime(n+1) = 1 and |s| is minimal.
 *            The function returns the integer t for the given index n (offset
 *            1)
 * Range:     i8 n <= 56, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   ext_gcd_ll, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility to obtain the first N primes. */
#include "../utils/sieve_first_n_primes.h"
#include "../utils/ext_gcd_ll.h"

static bool primes_ready = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void ensure_primes(size_t max_n)
{
    /* We need primes up to index max_n (1‑based). */
    size_t needed = max_n + 1;  /* primes[0] = prime(1) */
    if (!primes_ready || prime_count < needed) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        sieve_first_n_primes((idx_t)needed, &prime_list, &prime_count);
        primes_ready = true;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based. */
    if (n < 1) return (seq_t)0;  /* undefined, but avoid UB */

    ensure_primes((size_t)n + 1);

    long long p = prime_list[n - 1];       /* prime(n)   */
    long long q = prime_list[n];          /* prime(n+1) */

    long long x, y, g;
    ext_gcd_ll(p, q, &x, &y, &g);         /* p*x + q*y = 1 */

    /* Compute the minimal‑|s| solution. */
    long long s0 = x % q;
    if (s0 < 0) s0 += q;                  /* 0 <= s0 < q */
    long long t0 = (1 - s0 * p) / q;       /* integer division */

    long long s_opt = s0;
    long long t_opt = t0;
    long long s_alt = s0 - q;
    long long t_alt = t0 + p;

    if (labs(s_alt) < labs(s_opt)) {
        s_opt = s_alt;
        t_opt = t_alt;
    }

    return (seq_t)t_opt;
}
