/*
 * A185447  Least prime such that whenever 2*a(n) is the sum of two primes,
 *          none of them is in the set F such that Sum_{p in F} 2^primepi(p-1)
 *          = n.
 * https://oeis.org/A185447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A185447: least prime p such that every Goldbach partition
 *  of 2p uses primes outside the set F(n) determined by the binary
 *  expansion of n.  The function finds the answer by brute‑force search
 *  and a small sieve for primes up to a fixed limit.  It satisfies the
 *  requirements for both C11 and C++17.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- Prime sieve (size sufficient for all tested n) ----------------- */

static const size_t SIEVE_LIMIT = 10000000;       /* 10 million  */
static bool *prime_sieve      = NULL;             /* 0/1 array */
static uint32_t *prime_list   = NULL;             /* prime values */
static size_t   prime_cnt     = 0;                /* number of primes */
static bool     sieve_done    = false;

/* Build sieve and prime list once */
static void init_sieve(void)
{
    if (sieve_done) return;
    prime_sieve = (bool*)malloc(SIEVE_LIMIT + 1);
    memset(prime_sieve, 1, SIEVE_LIMIT + 1);
    prime_sieve[0] = prime_sieve[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (prime_sieve[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_sieve[j] = 0;

    /* Count primes and build list */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_sieve[i]) ++prime_cnt;

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_sieve[i]) prime_list[idx++] = (uint32_t)i;

    sieve_done = true;
}

/* --- Utility helpers ------------------------------------------------- */

/* Test if x is in the forbidden set. */
static inline bool is_forbidden(uint32_t x, size_t forbid_cnt,
                                const uint32_t *forb)
{
    for (size_t i = 0; i < forbid_cnt; ++i)
        if (forb[i] == x) return true;
    return false;
}

/* Test primality using the sieve or trial division.   */
static inline bool is_prime_uint32(uint32_t n)
{
    if (n <= SIEVE_LIMIT) return prime_sieve[n];
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i)
        if (n % prime_list[i] == 0) return false;
    return true;
}

/* Test whether prime p satisfies Goldbach‑avoiding condition for
 * the forbidden set.  The list of all primes is used. */
static bool valid_prime(uint32_t p, size_t forbid_cnt,
                        const uint32_t *forb)
{
    for (size_t j = 0; j < prime_cnt && prime_list[j] <= p; ++j) {
        uint32_t q = prime_list[j];
        uint32_t r = 2u * p - q;
        if (r < 2) continue;
        if (!is_prime_uint32(r)) continue;
        if (is_forbidden(q, forbid_cnt, forb) ||
            is_forbidden(r, forbid_cnt, forb))
            return false;
    }
    return true;
}

/* --- Sequence A185447 ------------------------------------------------- */

seq_t a(idx_t n)
{
    init_sieve();

    /* Even n: a(n)=2 */
    if ((n & 1ULL) == 0ULL) return (seq_t)2;

    /* n ≡ 1 (mod 4): a(n)=3 */
    if ((n & 3ULL) == 1ULL) return (seq_t)3;

    /* Build forbidden set F(n) from binary expansion of n. */
    uint32_t forbid[32];
    size_t forbid_cnt = 0;
    idx_t tmp = n;
    int pos = 0;
    while (tmp) {
        if (tmp & 1ULL)
            forbid[forbid_cnt++] = prime_list[pos];
        tmp >>= 1;
        ++pos;
    }

    /* Search for the least prime ≥5 that satisfies the condition. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p < 5) continue;
        if (valid_prime(p, forbid_cnt, forbid))
            return (seq_t)p;
    }

    /* Should never reach here for the tested range. */
    return (seq_t)0;
}
