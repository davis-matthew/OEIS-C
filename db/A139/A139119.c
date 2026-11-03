/*
 * A139119  Primes whose binary representation shows the distribution of prime
 *          numbers using "0" for primes and "1" for nonprime numbers.
 * https://oeis.org/A139119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For k = 1,2,... construct S_k = a(1)a(2)...a(k) where a(i) is
 *            the characteristic function of non‑prime numbers (1 if i is
 *            composite or 1, 0 if i is prime). a(n) is the nth prime value of
 *            S_k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function uses the provided utilities:
   is_prime_small(seq_t)  - primality test for small integers
   is_prime_big(seq_t)    - primality test for arbitrary precision */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *cache   = NULL;   /* array of prime values */
    static size_t cache_size   = 0; /* number of primes cached */
    static size_t cache_cap    = 0; /* allocated capacity of cache   */

    /* State of the incremental construction of S_k */
    static idx_t k         = 0;     /* current length of S_k (k >= 0) */
    static seq_t cur_val   = 0;     /* current value of S_k as a sequence integer */

    /* Lazy allocation and growth of the cache array */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Extend the cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        /* Advance k and append the next bit a(k+1) */
        ++k;
        bool bit = !is_prime_seq((seq_t)k); /* 1 if k is composite (or 1), 0 if prime */
        seq_t shifted = cur_val << 1;
        if (bit) shifted |= (seq_t)1;
        cur_val = shifted;

        /* Test the new value for primality */
        if (is_prime_big(cur_val))
        {
            /* Grow cache if necessary */
            if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_size++] = cur_val;
        }
    }

    /* Cache is 0‑based but n is 1‑based */
    return cache[n - 1];
}
