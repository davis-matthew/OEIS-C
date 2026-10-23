/*
 * A119905  Numbers k such that gcd(k, R_k) = 1, where R_k is the repunit
 *          (10^k -1)/9 = A002275(k).
 * https://oeis.org/A119905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: returns the n-th integer k such that gcd(k, R_k) = 1,
   where R_k = (10^k - 1)/9 is the k‑digit repunit.
   The predicate “valid” is tested by factorising k and checking,
   for every prime divisor p ≠ 3, whether 10^k ≡ 1 (mod p).  If so,
   p divides R_k, so the gcd is >1.  The algorithm is O(n · √k).
*/

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;      /* array of found values */
static size_t cache_len = 0;     /* number of values stored   */
static size_t cache_cap = 0;     /* allocated capacity         */

/* Append a value to the cache, expanding it if necessary. */
static void push_candidate(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return true iff gcd(k, R_k) == 1.  Uses prime factorisation of k. */
static bool is_valid(idx_t k)
{
    /* A factor 3 immediately gives gcd > 1 because 3 | R_k when k ≡ 0 (mod 3). */
    if ((k % 3) == 0) return false;

    idx_t temp = k;

    /* Skip factor 2 – it never divides a repunit. */
    while ((temp & 1) == 0) temp >>= 1;

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            /* p is a prime divisor of k (p ≠ 3 because k % 3 != 0). */
            seq_t powmod = mod_pow((seq_t)10, k, (seq_t)p);
            if (powmod == 1) return false;   /* p | R_k */
            while (temp % p == 0) temp /= p;
        }
    }

    if (temp > 1) {
        /* temp is a remaining prime factor (not 3). */
        seq_t powmod = mod_pow((seq_t)10, k, (seq_t)temp);
        if (powmod == 1) return false; /* temp | R_k */
    }

    return true; /* all prime factors satisfy the condition */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 safely */

    static idx_t next_k = 1;          /* next k to test */

    while (cache_len < n) {
        if (is_valid(next_k))
            push_candidate((seq_t)next_k);
        ++next_k;
    }

    return cache[n - 1];
}
