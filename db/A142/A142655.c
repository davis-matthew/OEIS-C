/*
 * A142655  Primes congruent to 33 mod 56.
 * https://oeis.org/A142655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 56 == 33. The function
 *            searches sequentially for primes of the form 56·k+33 and caches
 *            the results
 * Range:     u8 n <= 1, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 989 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *prime_cache = NULL; /* buffer of found primes */
static size_t cache_cap  = 0;     /* allocated capacity of prime_cache */
static size_t cache_n    = 0;     /* number of primes stored in prime_cache */

/* Ensure that prime_cache can hold at least 'needed' elements. */
static void enlarge_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp)
        abort(); /* out of memory – cannot continue */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Append a newly found prime to the cache. */
static void append_prime(seq_t p)
{
    enlarge_cache(cache_n + 1);
    prime_cache[cache_n++] = p;
}

/* Return the n-th prime ≡ 33 (mod 56).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n == 0 */

    /* 'next_k' is the current k value for the candidate 56·k+33.
       It starts at 1 because 56·0+33 = 33 is not prime. */
    static idx_t next_k = 1;

    /* Generate more primes until we have at least n of them. */
    for ( ; cache_n < n; ++next_k )
    {
        idx_t cand_idx = 33 + next_k * 56; /* candidate fits in idx_t */
        seq_t candidate = (seq_t)cand_idx;

        if (is_prime_big(candidate))
            append_prime(candidate);
    }

    /* Index cache_n-1 corresponds to the n-th prime. */
    return prime_cache[n - 1];
}
