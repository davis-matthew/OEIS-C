/*
 * A233249  a(1)=0; for k >= 1, let prime(k) map to 10...0 with k-1 zeros and
 *          let prime(k)*prime(m) map to the concatenation in binary of
 *          2^(k-1) and 2^(m-1). For n >= 2, let the prime power factorization
 *          of n be mapped to r(n). a(n) is the term in A114994 which is
 *          c-equivalent to r(n) (see there our comment).
 * https://oeis.org/A233249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A233249:  a(n) is the minimal binary concatenation
 * of blocks 2^(k-1) (k = prime index of each prime factor of n,
 * repeated according to the exponent).  The blocks are sorted
 * in descending order of k to obtain the minimal value. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* static prime list, initialized once */
static uint64_t *prime_list = NULL;
static size_t      prime_cnt  = 0;
static int        primes_inited = 0;

/* initialise prime list up to 10 000 (enough for n ≤ 10 000) */
static void
init_primes(void)
{
    if (primes_inited) return;
    build_primes_upto(10000, &prime_list, &prime_cnt);
    primes_inited = 1;
}

/* binary search: return 1‑based index of prime p (assumed to exist) */
static idx_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] == p) return (idx_t)(mid + 1);
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* should never happen for valid input */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;           /* by definition */

    init_primes();                  /* ensure primes available */

    /* factor n and collect block lengths (prime indices) */
    uint64_t tmp = (uint64_t)n;
    idx_t block_len[32];            /* enough for n ≤ 10 000 */
    size_t len = 0;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            size_t exp = 0;
            do {
                tmp /= p;
                exp++;
            } while (tmp % p == 0);
            idx_t k = (idx_t)(i + 1);          /* prime index (1‑based) */
            for (size_t j = 0; j < exp; ++j)
                block_len[len++] = k;
        }
    }
    if (tmp > 1) {   /* remaining prime factor > sqrt(n) */
        idx_t k = prime_index(tmp);
        block_len[len++] = k;
    }

    /* sort block lengths in descending order */
    for (size_t i = 0; i < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (block_len[j] > block_len[i]) {
                idx_t t = block_len[i];
                block_len[i] = block_len[j];
                block_len[j] = t;
            }

    /* build the concatenated binary number */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        idx_t k = block_len[i];
        seq_t block = ((seq_t)1) << ((idx_t)k - 1);  /* 2^(k-1) */
        res = (res << k) | block;
    }

    return res;
}
