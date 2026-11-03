/*
 * A130090  Primes prime(n) such that at least one of the two numbers
 *          (prime(n+1)^2-prime(n)^2)/2 - 1 and (prime(n+1)^2-prime(n)^2)/2 +
 *          1 is not prime.
 * https://oeis.org/A130090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130090: a(n) is the n-th prime p for which at least one of  
   ((prime(n+1)^2 - prime(n)^2)/2 - 1) or ((prime(n+1)^2 - prime(n)^2)/2 + 1)  
   is composite.  This function builds the sequence lazily and memoises
   results so repeated calls are fast.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

/* Return the n-th term of A130090 (1‑based).  */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;           /* array of qualifying primes   */
    static size_t cache_len = 0;          /* number of elements in cache */
    static size_t cache_cap = 0;          /* allocated capacity          */

    /* Static state for continued prime generation between calls. */
    static seq_t prev_prime = 0;          /* prime(n) from last iteration */
    static idx_t  current_k  = 0;         /* index of prev_prime (prime(current_k)) */
    static bool    first_time = true;     /* initialise on first call */

    /* Grow or initialise the cache array on first use. */
    if (first_time) {
        first_time = false;
        /* Prime 1 is 2, prime 2 is 3, etc.  */
        prev_prime = (seq_t)get_prime(1);   /* p(1) = 2 */
        current_k  = 1;
    }

    /* Expand the required number of cache entries.  */
    size_t target = (size_t)n;
    if (cache_len < target) {
        /* Ensure the cache can grow.  */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        while (cache_len < target) {
            /* Get the next prime.  */
            ++current_k;                       /* prime index for next.   */
            seq_t curr_prime = (seq_t)get_prime(current_k);

            /* Compute (curr^2 - prev^2) / 2 if divisible by 2.  */
            seq_t diff = curr_prime * curr_prime - prev_prime * prev_prime;
            if ((diff % 2) == 0) {
                seq_t v = diff / 2;
                seq_t v_minus = v - 1;
                seq_t v_plus  = v + 1;
                if (!is_prime_big(v_minus) || !is_prime_big(v_plus)) {
                    /* prev_prime satisfies the condition.  */
                    if (cache_len + 1 > cache_cap) {
                        cache_cap *= 2;
                        cache = mem_resize(seq_t, cache, cache_cap);
                    }
                    cache[cache_len++] = prev_prime;
                }
            }
            /* Slide window: prev becomes curr for next pair.  */
            prev_prime = curr_prime;
        }
    }

    /* Return the requested term.  n is 1‑based.  */
    return cache[target - 1];
}
