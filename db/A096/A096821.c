/*
 * A096821  Solutions to Mod[sigma(x),x]=12 of the form p*(p+13)/2 where p is
 *          a prime of form=2^j-13, of which j exponents are listed in
 *          A096818.
 * https://oeis.org/A096821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p+13)/2 where p = 2^j-13 is prime and j is the n‑th
 *            exponent for which 2^j-13 is prime
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
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

/* Cache of previously computed terms.  The array is resized as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_size = 0;  /* number of terms stored */
static idx_t next_j = 4;       /* next exponent to test; first j where 2^j-13 > 0 */

seq_t a(idx_t n)
{
    /* Return cached value if already available. */
    if (n >= 1 && (size_t)n <= cache_size) {
        return cache[(size_t)n - 1];
    }

    /* Ensure we have enough capacity for new terms. */
    if (cache_cap <= cache_size) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_size < (size_t)n) {
        /* Compute p = 2^j - 13.  j starts at 4 where this is >0. */
        seq_t p = pow2(next_j) - (seq_t)13;

        /* Only consider prime p. */
        if (is_prime_big(p)) {
            /* a = p * (p+13) / 2.  (p+13) = 2^j is even. */
            seq_t val = p * ((p + (seq_t)13) / 2);
            cache[cache_size++] = val;
        }

        ++next_j;   /* move to next exponent */
    }

    return cache[(size_t)n - 1];
}
