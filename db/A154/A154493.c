/*
 * A154493  a(n+1)-+a(n)=prime, a(n+1)*a(n)=Average of twin prime pairs,
 *          a(1)=1,a(2)=4.
 * https://oeis.org/A154493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A154493: a(1)=1, a(2)=4.
 * For n ≥ 2, a(n+1) is defined to be the smallest integer > a(n) such that:
 *   1. a(n+1) - a(n)  is prime,
 *   2. a(n+1) + a(n)  is prime,
 *   3. a(n+1) * a(n) is the average of a twin prime pair
 *      (i.e. t-1 and t+1 are both prime, where t = a(n+1)*a(n)).
 *
 * The function a(idx_t n) implements this definition; it memoizes computed
 * values to allow arbitrary-order calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* allocate or extend a static cache of sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;   /* number of terms currently stored (offset 1) */

/* forward declaration of helper that finds the next term */
static seq_t next_term(seq_t prev);

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 0 */
    if (cache_size < (size_t)n) {
        /* extend the cache to size n */
        size_t new_size = (size_t)n;
        seq_t *new_cache = mem_resize(seq_t, seq_cache, new_size);
        if (!new_cache) {
            /* allocation failure: abort program */
            abort();
        }
        seq_cache = new_cache;
        /* compute missing values */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)            /* a(1) */
                seq_cache[i] = (seq_t)1;
            else if (i == 1)        /* a(2) */
                seq_cache[i] = (seq_t)4;
            else
                seq_cache[i] = next_term(seq_cache[i - 1]); /* compute a(i+1) from a(i) */
        }
        cache_size = new_size;
    }
    return seq_cache[n - 1];
}

/* Find the next term given the previous term 'prev' (= a(n)) */
static seq_t next_term(seq_t prev)
{
    seq_t candidate = prev + 1;           /* smallest integer > prev */
    while (true) {
        seq_t diff = candidate - prev;    /* a(n+1) - a(n) */
        if (!is_prime_big(diff))
        {
            ++candidate;
            continue;
        }

        seq_t sum = candidate + prev;    /* a(n+1) + a(n) */
        if (!is_prime_big(sum))
        {
            ++candidate;
            continue;
        }

        seq_t product = prev * candidate;   /* a(n+1) * a(n) */
        if (is_prime_big(product - 1) && is_prime_big(product + 1))
            return candidate;               /* all conditions satisfied */

        ++candidate;
    }
}
