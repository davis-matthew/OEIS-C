/*
 * A234851  Indices of primes in A014692, i.e., numbers k such that
 *          prime(k)-k+1 is prime.
 * https://oeis.org/A234851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that prime(k) − k + 1 is prime.
 *            The routine builds the list of qualifying indices on demand,
 *            memoizing results so that calls in any order remain fast
 * Range:     u8 n <= 15, u16 n <= 2086, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9652 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
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

static seq_t *cache    = NULL;   /* cache of a(n) values          */
static size_t cache_len = 0;     /* number of values stored        */
static size_t cache_cap = 0;     /* allocated capacity             */

static seq_t *primes   = NULL;   /* list of prime(k) values        */
static size_t primes_len = 0;    /* number of primes known         */
static size_t primes_cap = 0;    /* allocated capacity             */

static idx_t  next_k      = 1;   /* next k to examine              */
static idx_t  next_candidate = 2;/* next integer to test for prime */

static void grow_seq(seq_t **arr, size_t *len, size_t *cap, size_t new_len)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        while (new_cap < new_len) new_cap <<= 1;
        *arr = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Ensure n is at least 1.  Behaviour for n < 1 is undefined.  */
    size_t target = (size_t)n;

    while (cache_len < target) {
        /*--- Grow primes list until we have prime(next_k) ---*/
        while (primes_len < (size_t)next_k) {
            /* Find next prime after current candidate. */
            while (!is_prime(next_candidate))
                ++next_candidate;
            /* Append the prime to the array.   */
            grow_seq(&primes, &primes_len, &primes_cap, primes_len + 1);
            primes[primes_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }

        /*--- Check the condition for current k ---*/
        seq_t pk = primes[(size_t)next_k - 1];  /* prime(k)   */
        seq_t val = pk - (seq_t)next_k + 1;     /* candidate test value */

        if (is_prime_big(val)) {
            grow_seq(&cache, &cache_len, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return cache[target - 1];
}
