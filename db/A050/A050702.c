/*
 * A050702  Nonprime numbers n such that n and n-reversed (<>n and no leading
 *          zeros) have the same number of prime factors and these prime
 *          factors (palindromes allowed here) are also reversals of each
 *          other.
 * https://oeis.org/A050702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 200, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   check_rev_property, rev_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050702: 
   nonprime n (n ≠ rev(n)) such that n and rev(n) have the same number
   of distinct prime factors and for every prime factor p of n, rev(p)
   is a prime factor of rev(n).  “rev” means decimal reversal with all
   leading zeros removed.  The sequence starts with 26, 39, 62, 93, … */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/check_rev_property.h"
#include "../utils/rev_uint64.h"

/* ---------- prime sieve utilities ---------------------------------- */
static uint64_t *prime_list = NULL;   /* primes discovered so far */
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;
static uint64_t last_checked = 1;     /* highest number already tested */

/* ensure all primes up to (and including) `limit` are generated */
static void ensure_primes_up_to(uint64_t limit)
{
    if (last_checked >= limit) return;
    for (uint64_t n = last_checked + 1; n <= limit; ++n) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > n / p) break;          /* p*p > n */
            if (n % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = n;
        }
    }
    last_checked = limit;
}

/* integer square‑root, safe for 64‑bit values */
static uint64_t int_sqrt(uint64_t n)
{
    uint64_t x = (uint64_t)sqrt((long double)n);
    /* adjust if off by one due to floating error */
    while ((unsigned __int128)x * x > n)            --x;
    while ((unsigned __int128)(x + 1) * (x + 1) <= n) ++x;
    return x;
}

/* --------------------------------------------------------------- */

/* reverse decimal digits of `x` (leading zeros are discarded) */

/* factor distinct primes of `n`;  `fac` (size 16) receives them,
   `cnt` is set to the number of distinct primes.  Assumes `n≥2`. */
static void factor_distinct(uint64_t n, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    uint64_t temp = n;
    uint64_t lim   = int_sqrt(temp);
    ensure_primes_up_to(lim);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > lim) break;
        if (temp % p == 0) {
            fac[(*cnt)++] = p;
            while (temp % p == 0) temp /= p;
            lim = int_sqrt(temp);
        }
    }
    if (temp > 1) fac[(*cnt)++] = temp;
}

/* dynamic array to hold the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;

static void seq_push(seq_t v)
{
    if (seq_cache_len >= seq_cache_cap) {
        seq_cache_cap = (seq_cache_cap == 0) ? 16 : seq_cache_cap * 2;
        seq_cache = (seq_t*)realloc(seq_cache,
                                    seq_cache_cap * sizeof(seq_t));
    }
    seq_cache[seq_cache_len++] = v;
}

/* ---------- the required sequence function -----------------------

   The function uses static storage for the cache and for the prime
   list, so successive calls keep a record of already found terms.
   It works correctly for any `n >= 1`.  Each term is computed by
   brute‑force testing of successive integers until `n` terms have
   been collected.

   The algorithm is O(n √m) where m is the magnitude of the
   n‑th element (roughly 10⁶ for n=10 000), well below the 2 s
   limit for the given bounds. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for n < 1 */

    static uint64_t cand = 2;                /* next integer to test */

    while (seq_cache_len < n) {
        /* a nonprime, not a palindrome */
        uint64_t revc = rev_uint64(cand);
        if (cand != revc) {
            uint64_t fac[16], fac_rev[16];
            size_t cnt = 0, cnt_rev = 0;
            factor_distinct(cand, fac, &cnt);
            /* skip primes */
            if (!(cnt == 1 && fac[0] == cand)) {
                factor_distinct(revc, fac_rev, &cnt_rev);
                if (cnt == cnt_rev && check_rev_property(fac, cnt, fac_rev, cnt_rev)) {
                    seq_push((seq_t)cand);
                }
            }
        }
        ++cand;
    }
    return seq_cache[n-1];
}
