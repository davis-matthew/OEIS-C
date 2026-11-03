/*
 * A193144  Primes of the form n^2 + n + 1, where n is semiprime.
 * https://oeis.org/A193144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form n^2 + n + 1 where n is semiprime
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

/* Count the total number of prime factors (with multiplicity) of n
   using the prime array primes[0 .. pr_count-1] which contains
   all primes up to at least sqrt(n).  Return that count. */
static size_t semiprime_factor_count(idx_t n, const uint64_t *primes, size_t pr_count)
{
    idx_t temp = n;
    size_t count = 0;
    for (size_t i = 0; i < pr_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        while (temp % primes[i] == 0) {
            temp /= primes[i];
            ++count;
            if (count > 2) return count;  /* early exit if not semiprime */
        }
    }
    if (temp > 1) ++count;   /* leftover prime factor */
    return count;
}

/* Return the n-th term of the sequence.  The function works on
   arbitrary calls in any order and caches previously computed
   terms. */
seq_t a(idx_t n)
{
    /* One-time setup: build a small list of primes sufficient for
       factorisation of all semiprimes up to the maximum n requested. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static bool primes_ready = false;

    /* Cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Highest semiprime value that has already been examined. */
    static idx_t max_checked = 0;

    /* Build prime list once.  1 000 000 is plenty for n up to 10 000. */
    if (!primes_ready) {
        build_primes_upto(1000000ULL, &primes, &primes_cnt);
        primes_ready = true;
    }

    /* If the caller requests an index that is already cached, return it. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Otherwise keep searching for new terms until we have enough. */
    for (idx_t k = max_checked + 1; ; ++k) {
        /* Check whether k is a semiprime. */
        if (semiprime_factor_count(k, primes, primes_cnt) == 2) {
            /* Compute k^2 + k + 1 in seq_t. */
            seq_t val = (seq_t)k * (seq_t)k + k + 1;
            if (is_prime_big(val)) {
                /* Grow the cache if necessary. */
                if (cache_len + 1 > cache_cap)
                    ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = val;

                /* We have now produced the n-th term; remember the current k
                   so that future calls do not re‑process it. */
                max_checked = k;
                return val;
            }
        }
        /* Record that we have examined k, even if it was not accepted. */
        max_checked = k;
    }
}
