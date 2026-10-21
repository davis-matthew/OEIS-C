/*
 * A207292  Polya-Vinogradov numbers A177865 for primes p == 3 (mod 4).
 * https://oeis.org/A207292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Polya–Vinogradov numbers for primes p == 3 (mod 4)
   a(n) = max_{0<k<p} | sum_{i=1..k} L(i/p) |   where p is the n‑th prime ≡3 (mod 4)   */
/* Implementation uses a simple prime sieve by trial division and
   marks quadratic residues by squaring.  The function is memoised,
   so any order of calls is supported.  It runs in O(p) time per prime
   and stays well under the time limit for n ≤ 10 000.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------------
   Utility: integer square root (floor(sqrt(n)))                      ------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* ------------------------------------------------------------------------
   Prime list (stores all primes found so far)                        ------- */
#ifdef __cplusplus
static std::vector<uint64_t> prime_list_vec;
#else
static uint64_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static size_t prime_list_cap = 0;
#endif

static void add_prime(uint64_t p)
{
#ifdef __cplusplus
    prime_list_vec.push_back(p);
#else
    if (prime_list_cnt == prime_list_cap) {
        prime_list_cap = prime_list_cap ? prime_list_cap * 2 : 1;
        prime_list = (uint64_t *)realloc(prime_list,
                                         prime_list_cap * sizeof(uint64_t));
    }
    prime_list[prime_list_cnt++] = p;
#endif
}

/* ------------------------------------------------------------------------
   Generate the next prime after the last one found.                     ------- */
static uint64_t last_prime = 2;   /* smallest prime */

static uint64_t next_prime(void)
{
    uint64_t cand = last_prime + 1;
    if (cand <= 2) cand = 2;
    if ((cand & 1ULL) == 0ULL && cand != 2ULL) cand++;   /* skip evens */

    while (true) {
        /* trial division by known primes up to sqrt(cand) */
        uint64_t r = isqrt_u64(cand);
        bool ok = true;
        size_t i;
        for (i = 0; i < /* number of primes */ (
#ifdef __cplusplus
            prime_list_vec.size()
#else
            prime_list_cnt
#endif
           ); i++) {
#ifdef __cplusplus
            uint64_t q = prime_list_vec[i];
#else
            uint64_t q = prime_list[i];
#endif
            if (q > r) break;
            if (cand % q == 0) { ok = false; break; }
        }
        if (ok) {
            add_prime(cand);
            last_prime = cand;
            return cand;
        }
        cand += 2;   /* only odd candidates */
    }
}

/* ------------------------------------------------------------------------
   Residue buffer (1 byte per residue modulo current prime)            ------- */
static uint8_t *residue_buf = NULL;
static size_t residue_cap = 0;

static void ensure_residue_capacity(uint64_t p)
{
    if (residue_cap < (size_t)p) {
        free(residue_buf);
        residue_buf = (uint8_t *)malloc((size_t)p);
        residue_cap = (size_t)p;
    }
    memset(residue_buf, 0, (size_t)p);
}

/* ------------------------------------------------------------------------
   Compute Polya number for a single prime p == 3 (mod 4)                 ------- */
static seq_t compute_for_prime(uint64_t p)
{
    ensure_residue_capacity(p);

    /* mark quadratic residues: a^2 mod p for a = 1 .. (p-1)/2 */
    uint64_t half = (p - 1) >> 1;
    for (uint64_t a = 1; a <= half; ++a) {
        uint64_t sq = (a * a) % p;
        residue_buf[sq] = 1;
    }

    /* scan partial sums of Legendre symbols */
    int sum = 0;
    int max_abs = 0;
    for (uint64_t i = 1; i < p; ++i) {
        sum += residue_buf[i] ? 1 : -1;
        int ab = sum < 0 ? -sum : sum;
        if (ab > max_abs) max_abs = ab;
    }
    return (seq_t)max_abs;
}

/* ------------------------------------------------------------------------
   Cache of computed Polya–Vinogradov numbers (1 per 3‑mod‑4 prime)   ------- */
#ifdef __cplusplus
static std::vector<seq_t> a_cache_vec;
#else
static seq_t *a_cache = NULL;
static size_t a_cache_cnt = 0;
static size_t a_cache_cap = 0;
#endif

static void store_a_value(seq_t val)
{
#ifdef __cplusplus
    a_cache_vec.push_back(val);
#else
    if (a_cache_cnt == a_cache_cap) {
        a_cache_cap = a_cache_cap ? a_cache_cap * 2 : 1;
        a_cache = (seq_t *)realloc(a_cache,
                                   a_cache_cap * sizeof(seq_t));
    }
    a_cache[a_cache_cnt++] = val;
#endif
}

/* ------------------------------------------------------------------------
   Public function required by the build interface                       ------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 – return 0 for safety */

    /* ensure we have computed at least n values */
    while (true) {
#ifdef __cplusplus
        size_t count = a_cache_vec.size();
#else
        size_t count = a_cache_cnt;
#endif
        if (count >= (size_t)n) break;

        /* generate next prime (may be 2, then 3, etc.) */
        uint64_t p = next_prime();
        if ((p & 3ULL) != 3ULL) continue;   /* skip primes not ≡3 mod 4 */

        /* compute Polya number for this prime and cache it */
        seq_t val = compute_for_prime(p);
        store_a_value(val);
    }

    /* retrieve the requested value */
#ifdef __cplusplus
    return a_cache_vec[n - 1];
#else
    return a_cache[n - 1];
#endif
}
