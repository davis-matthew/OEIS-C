/*
 * A165292  Primes obtained from other primes by pre-concatenating with 3.
 * https://oeis.org/A165292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   * a(n) is the n‑th prime of the form 3·10^k + p where p is
 *            itself a prime. * The function generates values by scanning
 *            successive primes p and * testing the concatenated number for
 *            primality
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Provided helper functions */
#include <math.h>
#include "../utils/num_digits0.h"
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

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Next prime candidate to test */
static uint64_t next_candidate = 2;

/* Append a value to the cache, growing it if needed */
static void cache_push(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return 10^(number of decimal digits of n) as a seq_t */
static seq_t pow10_of_digits(uint64_t n)
{
    size_t d = num_digits0(n);
    seq_t p = 1;
    for (size_t i = 0; i < d; ++i)
        p *= (seq_t)10;
    return p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                         /* undefined for n < 1 */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* Test whether the current candidate is prime */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t p10 = pow10_of_digits(next_candidate);
            seq_t q = (seq_t)next_candidate + ((seq_t)3) * p10;
            if (is_prime_big(q))
                cache_push(q);
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
