/*
 * A065244  Primes of form p = 2 + Sum_{k = 1..m} k^2 for some m.
 * https://oeis.org/A065244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + Σ_{k=1..m} k² where the resulting number is prime
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds successive values of m that produce primes and
   caches the primes.  It uses a dynamic array, growing it as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

/* Cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;   /* allocated capacity */
static size_t cache_size  = 0;   /* number of primes stored */
static idx_t  last_m      = 0;   /* last m processed */

/* Return the nth prime of the form 2 + Σk² (n >= 1).  */
seq_t a(idx_t n)
{
    /* Grow the cache until it has at least n primes. */
    while (cache_size < n)
    {
        /* Next candidate m. */
        last_m++;
        idx_t m = last_m;

        /* Σk² = m*(m+1)*(2m+1)/6. */
        idx_t sumsq = m * (m + 1) * (2 * m + 1) / 6;
        idx_t cand  = 2 + sumsq;

        /* If the candidate exceeds what can be represented in seq_t,
           stop expanding further because all later candidates are larger. */
        if (cand > (idx_t)((seq_t)-1))
            break;

        seq_t val = (seq_t)cand;

        /* Test for primality. */
        if (is_prime_big(val))
        {
            /* Expand cache if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
                seq_t *tmp = mem_new(seq_t, new_cap);
                if (!tmp) { /* allocation failed – abort silently */ return 0; }
                if (prime_cache)
                {
                    mem_copy(tmp, prime_cache, cache_size * sizeof(*tmp));
                    mem_free(prime_cache);
                }
                prime_cache = tmp;
                cache_cap   = new_cap;
            }

            prime_cache[cache_size++] = val;
        }
    }

    if (n == 0 || cache_size == 0) return 0;
    return prime_cache[n - 1];
}
