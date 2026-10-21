/*
 * A084360  Number of partitions of n into pair of parts whose difference is a
 *          prime.
 * https://oeis.org/A084360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1622, u16 n <= 8823, u32 n <= 8823, u64 n <= 8823,
 *            u128 n <= 8823, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A084360
 *  a(n) = 0, if n = 1,2,3
 *  a(n) = 1, if n is even and n ≥ 4
 *  a(n) = π(n−1) − 1, if n is odd and n ≥ 5
 *
 *  π(k) is the prime‑counting function (number of primes ≤ k).
 *  The function below computes π(k) via a dynamic sieve that expands
 *  as needed.  All arithmetic is performed with integer types only.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ----------  dynamic sieve ---------------- */

static uint8_t *prime_flag   = NULL;   /* 0 = prime, 1 = composite */
static idx_t  *prime_list   = NULL;   /* list of discovered primes   */
static uint32_t *prime_pi   = NULL;   /* π(n) for all n ≤ current limit */
static size_t   max_limit   = 0;       /* sieve range [0, max_limit]  */
static size_t   prime_cnt   = 0;       /* number of primes in prime_list */
static size_t   prime_cap   = 0;       /* capacity of prime_list         */

/* Expand the sieve so that it contains all numbers up to 'limit' (inclusive). */
static void extend_sieve(idx_t limit)
{
    if (limit <= max_limit)                 /* already covered */
        return;

    /* 1. Ensure prime_flag array is large enough */
    if (max_limit == 0) {
        prime_flag = (uint8_t*)calloc((size_t)limit + 1, 1);
        if (!prime_flag) exit(1);
    } else {
        prime_flag = (uint8_t*)realloc(prime_flag, (size_t)limit + 1);
        if (!prime_flag) exit(1);
        /* newly allocated region starts at max_limit+1 */
        size_t added = (size_t)limit - max_limit;
        memset(prime_flag + max_limit + 1, 0, added);
    }

    /* 2. Ensure prime_pi array is large enough */
    if (max_limit == 0) {
        prime_pi = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
        if (!prime_pi) exit(1);
    } else {
        prime_pi = (uint32_t*)realloc(prime_pi, (size_t)(limit + 1) * sizeof(uint32_t));
        if (!prime_pi) exit(1);
    }

    /* 3. Mark multiples of already known primes that fall into the new range */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > limit) break;
        idx_t start = (max_limit < (size_t)(p * p))
                      ? p * p
                      : ((max_limit / p + 1) * p);
        for (idx_t x = start; x <= limit; x += p)
            prime_flag[x] = 1;
    }

    /* 4. Find new primes in [max_limit+1 .. limit] */
    idx_t start_x = (max_limit < 2) ? 2 : max_limit + 1;
    for (idx_t x = start_x; x <= limit; ++x) {
        if (!prime_flag[x]) {                     /* x is prime */
            if (prime_cnt == prime_cap) {          /* grow array if needed */
                prime_cap = prime_cap ? prime_cap << 1 : 16;
                prime_list = (idx_t*)realloc(prime_list, prime_cap * sizeof(idx_t));
                if (!prime_list) exit(1);
            }
            prime_list[prime_cnt++] = x;

            /* 5. Mark multiples of this new prime (if they fit) */
            if (x * x <= limit) {
                for (idx_t y = x * x; y <= limit; y += x)
                    prime_flag[y] = 1;
            }
        }
    }

    /* 6. Build cumulative prime counting array prime_pi */
    prime_pi[0] = 0;
    if (limit >= 1) prime_pi[1] = 0;            /* 0 and 1 are not primes */
    for (size_t i = 2; i <= limit; ++i)
        prime_pi[i] = prime_pi[i-1] + (prime_flag[i] == 0 ? 1U : 0U);

    max_limit = (size_t)limit;
}

/* Return π(k), the number of primes ≤ k.  k ≥ 0. */
static idx_t pi_count(idx_t k)
{
    if (k < 2)
        return 0;
    extend_sieve(k);
    return prime_pi[k];
}

/* ----------  sequence function ----------- */

/* Term a(n) for OEIS A084360 */
seq_t a(idx_t n)
{
    /* small n handling */
    if (n <= 3)
        return 0;

    if ((n & 1) == 0)          /* even n ≥ 4 */
        return (seq_t)1;

    /* odd n ≥ 5: π(n−1) − 1 */
    idx_t cnt = pi_count(n - 1);

    /* cnt is guaranteed to be ≥ 1 here, so cnt - 1 fits in seq_t */
    return (seq_t)(cnt - 1);
}
