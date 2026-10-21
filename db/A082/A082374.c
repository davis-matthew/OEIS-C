/*
 * A082374  Numbers k such that the congruence prime(k+1)^x - prime(k)^x ==
 *          prime(k+2) mod prime(k+3) does not have a solution.
 * https://oeis.org/A082374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A082374
    a(n) = the n-th integer k such that the congruence
        prime(k+1)^x - prime(k)^x ≡ prime(k+2) (mod prime(k+3))
    has no solution in positive integers x.  */

/*  The implementation uses a simple sieve to provide primes up to a
    fixed bound (2 000 000), then tests each candidate k by brute‑forcing
    the congruence for exponents x = 1 … prime(k+3)‑1.  Results are
    memoised so that calls with arbitrary indices are cheap.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*---------------------------------------------------------------------*/
/*  Prime sieve for numbers up to SIEVE_LIMIT                          */
/*---------------------------------------------------------------------*/

#define SIEVE_LIMIT 2000000U          /* 1.5 millionth prime ≈ 2 000 000 */

/* Global storage for the prime table (zero‑based index)                */
static uint32_t *prime_vals = NULL;
static size_t     prime_cnt  = 0;
static bool       sieve_inited = false;

static void build_prime_sieve(void)
{
    size_t sz = SIEVE_LIMIT + 1;
    char *comp = (char*)calloc(sz, 1);       /* 0 == prime, 1 == composite */
    if (!comp) return;                       /* memory error → abort */

    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (!comp[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                comp[j] = 1;

    /* Rough estimate of number of primes up to SIEVE_LIMIT */
    size_t est = SIEVE_LIMIT / 10 + 100;     /* plenty of space */
    prime_vals = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!prime_vals) { free(comp); return; }

    prime_cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!comp[i]) {
            if (prime_cnt == est) {          /* should not overflow */
                size_t new_est = est * 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_vals, new_est * sizeof(uint32_t));
                if (!tmp) { free(comp); return; }
                prime_vals = tmp;
                est = new_est;
            }
            prime_vals[prime_cnt++] = i;
        }

    free(comp);
}

static void ensure_sieve(void)
{
    if (!sieve_inited) {
        build_prime_sieve();
        sieve_inited = true;
    }
}

/*---------------------------------------------------------------------*/
/*  Test whether the congruence has at least one solution for a given k */
/*---------------------------------------------------------------------*/

static bool has_solution(uint32_t a, uint32_t b, uint32_t r, uint32_t m)
{
    /* a = prime(k+1), b = prime(k), r = prime(k+2), m = prime(k+3)     */
    /* We look for x > 0 with a^x - b^x ≡ r (mod m)                       */
    uint64_t base_a = a % m;
    uint64_t base_b = b % m;
    uint64_t cur_a  = base_a;   /* a^1 */
    uint64_t cur_b  = base_b;   /* b^1 */

    for (uint32_t x = 1; x < m; ++x) {
        uint64_t diff = (cur_a >= cur_b) ? (cur_a - cur_b) : (cur_a + m - cur_b);
        if (diff == r) return true;   /* a solution is found */

        /* advance to next exponent                                 */
        cur_a = (cur_a * base_a) % m;
        cur_b = (cur_b * base_b) % m;
    }
    return false;                    /* no solution for any x                */
}

/*---------------------------------------------------------------------*/
/*  Memoised storage of found k-values                                 */
/*---------------------------------------------------------------------*/

static seq_t *k_terms = NULL;      /* array of k that satisfy the property    */
static size_t t_count = 0;         /* current number of stored terms          */
static size_t t_cap   = 0;         /* allocated capacity of the array         */
static uint32_t next_k = 2;        /* candidate k to test; starts at 2         */

static void grow_terms(void)
{
    size_t new_cap = (t_cap == 0) ? 16 : t_cap * 2;
    seq_t *tmp = (seq_t*)realloc(k_terms, new_cap * sizeof(seq_t));
    if (!tmp) return;            /* memory error → keep old array          */
    k_terms = tmp;
    t_cap   = new_cap;
}

/*---------------------------------------------------------------------*/
/*  Public sequence function                                          */
/*---------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) is meaningless                 */
    ensure_sieve();

    /* generate terms until we have at least n of them               */
    while (t_count < n) {
        /* make sure we still have enough primes for k+3             */
        if ((size_t)(next_k + 3) >= prime_cnt) break;   /* should not happen */

        uint32_t pk  = prime_vals[next_k - 1];         /* prime(k)   */
        uint32_t pk1 = prime_vals[next_k];            /* prime(k+1) */
        uint32_t pk2 = prime_vals[next_k + 1];        /* prime(k+2) */
        uint32_t pk3 = prime_vals[next_k + 2];        /* prime(k+3) */

        /* NOTE: The congruence uses prime(k+1)^x - prime(k)^x, so a=pk1, b=pk */
        if (!has_solution(pk1, pk, pk2, pk3)) {
            if (t_count == t_cap) grow_terms();
            k_terms[t_count++] = (seq_t)next_k;
        }

        ++next_k;   /* advance to next k value                               */
    }

    /* If we couldn't generate enough terms (unlikely for the limits) */
    if (n > t_count) return (seq_t)0;
    return k_terms[n - 1];
}
