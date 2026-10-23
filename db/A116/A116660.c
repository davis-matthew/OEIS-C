/*
 * A116660  Integers in both sequences A075658 and A067028.
 * https://oeis.org/A116660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integers in both sequences A075658 and A067028
   The function returns the nth term (offset 1) of that sequence. */

/* ---------- helpers ---------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer square‑root, floor, for uint64_t */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---------- prime list, built incrementally ---------- */
static uint64_t *primes      = NULL;  /* array of primes found */
static size_t   primes_len   = 0;     /* number of primes stored */
static size_t   primes_cap   = 0;     /* capacity of array          */
static uint64_t prime_limit  = 0;     /* largest number examined */

/* ensure all primes <= limit are in the primes array */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        bool ok = true;
        uint64_t sq = isqrt_u64(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= sq; ++i) {
            if (cand % primes[i] == 0) { ok = false; break; }
        }
        if (ok) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
        }
    }
    prime_limit = limit;
}

/* primality test: uses the prime list above */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    uint64_t lim = isqrt_u64(n);
    ensure_primes_up_to(lim);
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > lim) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* composite test: >1 and not prime */
static bool is_composite_uint64(uint64_t n)
{
    return (n > 1) && !is_prime_uint64(n);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    static idx_t   max_checked  = 1;     /* largest tested integer */
    static seq_t * seq          = NULL;  /* cache of found terms    */
    static size_t  seq_len      = 0;     /* number of terms cached  */
    static size_t  seq_cap      = 0;     /* capacity of cache       */

    while (seq_len < n) {                /* generate until we have n terms */
        ++max_checked;
        uint64_t cand = (uint64_t)max_checked;
        if (cand < 2) continue;           /* skip 1 */

        /* factorization: count multiplicities (mult)
           and sum distinct primes (sum_distinct) */
        uint64_t temp = cand;
        uint64_t sum_distinct = 0;
        uint64_t mult         = 0;

        uint64_t lim = isqrt_u64(temp);
        ensure_primes_up_to(lim);

        for (size_t i = 0; i < primes_len && primes[i] <= lim; ++i) {
            uint64_t p = primes[i];
            if (temp % p == 0) {
                sum_distinct += p;
                do { temp /= p; ++mult; } while (temp % p == 0);
                lim = isqrt_u64(temp);
            }
        }
        if (temp > 1) {                  /* remaining prime factor */
            sum_distinct += temp;
            ++mult;
        }

        /* check both numbers are composite */
        if (is_composite_uint64(mult) && is_composite_uint64(sum_distinct)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cand;
        }
    }
    return seq[n - 1];
}
