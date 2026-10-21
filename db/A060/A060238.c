/*
 * A060238  a(n) = det(M) where M is an n X n matrix with M[i,j] = lcm(i,j).
 * https://oeis.org/A060238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Π_{p≤n} (1-p)^{⌊n/p⌋}
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(0)=1, a(1)=1 */

#include <stdlib.h>
#include <stdint.h>

/* fast integer power for seq_t (assumes non‑negative exp) */
static seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Sieve cache for primes up to the greatest n seen so far */
static int *prime_cache    = NULL;  /* array of primes */
static size_t prime_cap    = 0;     /* allocated capacity of prime_cache */
static size_t prime_cnt    = 0;     /* number of primes stored */
static int   prime_limit   = 0;     /* largest n for which sieve was performed */

/* Generate all primes ≤ N and store in the cache.  Subsequent calls
   with larger N extend the cache; calls with a smaller N reuse the
   existing cache. */
static void sieve_primes(int N)
{
    if (N <= prime_limit) return;

    int new_cap = N + 1;
    if (prime_cap < new_cap) {
        prime_cache = (int *)realloc(prime_cache, new_cap * sizeof(int));
        prime_cap   = new_cap;
    }

    char *is_comp = (char *)calloc(N + 1, sizeof(char));
    if (!is_comp) return;  /* out of memory – unlikely for N≤10000 */

    prime_cnt = 0;
    for (int i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            prime_cache[prime_cnt++] = i;
            if ((long long)i * i <= N) {
                for (int j = i * i; j <= N; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    prime_limit = N;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;

    /* Ensure we have all primes up to n */
    sieve_primes((int)n);

    /* 1. factorial part: n! */
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result = result * (seq_t)i;

    /* 2. product over primes and sign factor */
    int sign_parity = 0;                 /* keeps track of (-1)^s */
    for (size_t i = 0; i < prime_cnt && prime_cache[i] <= n; ++i) {
        int p   = prime_cache[i];
        idx_t e = n / p;                 /* floor(n/p) */
        if (e == 0) break;
        sign_parity += (int)e;
        seq_t base  = (seq_t)(p - 1);
        result *= pow_seq_t(base, (int)e);
    }

    if (sign_parity & 1) result = -result;

    return result;
}
