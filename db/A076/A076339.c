/*
 * A076339  Primes of the form 512*k+1.
 * https://oeis.org/A076339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 8287 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A076339 – primes of the form 512*k + 1
   Generation method: incremental enumeration of candidates
   (512*k+1) and testing for primality with the supplied
   big‑integer primality routine is_prime_big.
   The implementation keeps a static cache so that
   repeated calls (in any order) are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static cache data */
static seq_t *prime_cache = NULL;    /* array of found primes   */
static size_t   cache_cap  = 0;      /* allocated capacity     */
static size_t   cache_cnt  = 0;      /* number of primes stored*/
static idx_t   next_k      = 0;      /* next k value (512*k+1) */

/* enlarge the cache array if needed */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, prime_cache, newcap);
    if (!tmp) abort();              /* unrecoverable memory failure */
    prime_cache = tmp;
    cache_cap   = newcap;
}

/* ensure that at least 'n' primes of the required form are cached */
static void ensure_cache(idx_t n)
{
    if (cache_cnt >= (size_t)n) return;

    while (cache_cnt < (size_t)n) {
        seq_t cand = ((seq_t)512) * (seq_t)next_k + (seq_t)1;
        ++next_k;
        /* skip the trivial non‑prime 1 */
        if (cand <= (seq_t)1) continue;
        if (is_prime_big(cand)) {
            grow_cache(cache_cnt + 1);
            prime_cache[cache_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of range – not used by OEIS */
    ensure_cache(n);
    return prime_cache[n - 1];
}
