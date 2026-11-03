/*
 * A117528  Prime at which the cumulative sum in A117527 is prime.
 * https://oeis.org/A117528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A117528
 *  A prime at which the cumulative sum of floor(p*e) over all primes p
 *  up to that prime is itself prime.
 *  Definition: let S_k = Σ_{i=1..k} floor(p_i*e) where p_i is the i‑th prime.
 *  a(n) = p_k whenever S_k is prime.  Sequence is indexed from 1.
 *  The implementation generates primes in order, keeps a running sum and
 *  records the prime whenever the sum becomes prime.  Results are cached
 *  so arbitrary call order is O(n) in total.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
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

/* Constant e as long double for computing floor(p*e).  The error of a
 * 64‑bit double is far smaller than the distance to the next integer,
 * so casting to uint64_t is safe for all prime values that appear in
 * the range of interest. */
static const long double E_CONST = 2.7182818284590452353602874713527L;

/* Cached sequence values */
static seq_t *cache = NULL;        /* array of a(n) terms */
static size_t cache_cap = 0;       /* current allocation size */
static size_t cache_len = 0;       /* number of terms stored */

/* State for generating the sequence on demand */
static idx_t last_prime = 0;       /* last prime processed (0 == none) */
static seq_t cumulative_sum = 0;   /* Σ floor(p*e) up to last_prime */

/* Ensure cache can hold at least one more element. */
static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term (1‑based).  n==0 is undefined, function
 * returns 0 for that case. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Generate terms until we have at least n cached. */
    while (cache_len < n) {
        /* Find the next prime > last_prime. */
        idx_t p = last_prime + 1;
        while (!is_prime(p)) ++p;
        last_prime = p;

        /* Compute floor(p*e).  The result fits in uint64_t for all
         * primes that occur for n <= 10000, and the conversion to
         * seq_t works for big‑int build as well. */
        uint64_t term_uint = (uint64_t)floorl((long double)p * E_CONST);
        seq_t term = (seq_t)term_uint;

        /* Update cumulative sum. */
        cumulative_sum += term;

        /* If the sum is prime, record the current prime. */
        if (is_prime_big(cumulative_sum)) {
            grow_cache();
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
