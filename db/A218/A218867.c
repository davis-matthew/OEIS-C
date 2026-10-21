/*
 * A218867  Number of prime pairs {p,q} with p>q and {p-4,q+4} also prime such
 *          that p+(1+(n mod 6))q=n if n is not congruent to 4 (mod 6), and
 *          p-q=n and q<n/2 if n=4 (mod 6).
 * https://oeis.org/A218867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A218867: count prime pairs {p,q} with p>q, p-4 and q+4 also prime
   such that p+(1+(n mod 6))q = n (if n≠4 mod 6)
   or p-q = n and q < n/2 (if n ≡ 4 mod 6) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool *prime_bool   = NULL;   /* sieve array, true if prime */
static idx_t *primes      = NULL;   /* list of primes */
static size_t prime_cnt   = 0;      /* number of primes */
static size_t sieve_limit = 0;      /* current sieve size */

/* create or enlarge a prime sieve up to `limit` */
static void init_sieve(size_t limit) {
    if (limit <= sieve_limit) return;           /* already enough */
    if (prime_bool) free(prime_bool);
    if (primes)    free(primes);
    prime_bool = (bool*)malloc(limit + 1);
    memset(prime_bool, 1, limit + 1);
    prime_bool[0] = prime_bool[1] = false;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (prime_bool[p]) {
            for (size_t m = p * p; m <= limit; m += p) {
                prime_bool[m] = false;
            }
        }
    }
    /* count primes and build list */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_bool[i]) prime_cnt++;
    primes = (idx_t*)malloc(prime_cnt * sizeof(idx_t));
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_bool[i]) primes[pos++] = (idx_t)i;
    sieve_limit = limit;
}

/* helper: test primality using the sieve (low values) */
static bool is_prime_sieve(idx_t x) {
    if (x <= sieve_limit) return prime_bool[(size_t)x];
    /* numbers beyond the sieve are assumed prime false */
    if (x < 2) return false;
    for (size_t i = 2; (uintptr_t)i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* main function required by the problem statement */
seq_t a(idx_t n) {
    /* no results for invalid indices; offset starts at 1 */
    if (n < 1) return (seq_t)0;

    /* ensure the sieve covers all possible primes needed (up to ~2n+10) */
    size_t lim;
    if (n > ((size_t)-1) / 2 - 10) lim = SIZE_MAX;
    else lim = (size_t)n * 2 + 10;
    if (lim < 10) lim = 10;   /* minimum reasonable size */
    init_sieve(lim);

    idx_t mod6 = n % 6;
    seq_t count = 0;

    if (mod6 != 4) {
        /* k = 1 + (n mod 6)  => k in {1,2,3,4,6} */
        idx_t k = 1 + mod6;
        /* Iterate over prime q */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t q = primes[i];
            if (q > (n - 3) / k) break;   /* q too big, p would be <= 1 or <= q */
            idx_t p = n - k * q;
            if (p <= q) continue;        /* need p > q */
            if (!is_prime_sieve(p)) continue;
            /* check p-4 and q+4 primality */
            if (p <= 4) continue;
            if (!is_prime_sieve(p - 4)) continue;
            if ((q + 4) > sieve_limit) {
                /* fallback primality test beyond sieve */
                if (!is_prime_sieve(q + 4)) continue;
            } else if (!prime_bool[(size_t)(q + 4)]) continue;
            count++;
        }
    } else {
        /* n ≡ 4 mod 6: p - q = n and q < n/2 */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t q = primes[i];
            if (q > (n - 1) / 2) break;   /* q < n/2 */
            idx_t p = q + n;              /* p > q automatically */
            if (p > sieve_limit) {
                /* need explicit primality test */
                if (!is_prime_sieve(p)) continue;
            } else if (!prime_bool[(size_t)p]) continue;
            if (!is_prime_sieve(q)) continue;   /* q is prime by construction, but keep check */
            if (p <= 4) continue;            /* p-4 must be prime */
            if (!is_prime_sieve(p - 4)) continue;
            if ((q + 4) > sieve_limit) {
                if (!is_prime_sieve(q + 4)) continue;
            } else if (!prime_bool[(size_t)(q + 4)]) continue;
            count++;
        }
    }
    return count;
}
