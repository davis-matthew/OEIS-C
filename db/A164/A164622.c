/*
 * A164622  Primes p such that p*floor(p/2) - 4 and p*floor(p/2) + 4 are prime
 *          numbers.
 * https://oeis.org/A164622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A164622.
   A prime p is accepted if both p·⌊p/2⌋−4 and p·⌊p/2⌋+4 are also prime.
*/
#include <stdlib.h>
#include <stdbool.h>
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

/* Cache the sequence values so that repeated calls to a(idx_t) are fast. */
static seq_t *cache = NULL;          /* array of stored terms */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_candidate = 3;     /* next odd number to test */
static idx_t found_count = 0;        /* number of terms found in cache */

static void grow_cache(idx_t needed)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }
    while (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* Undefined for n=0; return 0 as placeholder */

    /* Ensure the cache is large enough. */
    if ((size_t)n > cache_cap)
        grow_cache(n);

    /* Generate terms until we have at least n of them. */
    while (found_count < n) {
        if (is_prime(next_candidate)) {
            seq_t p = (seq_t)next_candidate;
            seq_t m = p >> 1;                 /* floor(p/2) */
            seq_t t1 = p * m - 4;
            seq_t t2 = p * m + 4;
            if (is_prime_big(t1) && is_prime_big(t2)) {
                cache[found_count] = p;
                found_count++;
            }
        }
        next_candidate += 2; /* check next odd number */
    }

    return cache[n - 1];
}
