/*
 * A094879  Where 1's occur in A094830 (nonprimes that reach a prime in one
 *          step under iteration of "x -> x + sum of squares of digits of x").
 * https://oeis.org/A094879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th positive integer x (x≥1) such that * x is NOT
 *            prime and x + Σ(digit²) is prime. * The sequence lists such x in
 *            increasing order
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>                /* size_t */

#include <stdint.h>
#include "../utils/get_digits.h"
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* compute the sum of squares of the decimal digits of x */
static seq_t digit_square_sum(seq_t x)
{
    uint8_t buf[64];
    size_t len;
    get_digits(x, buf, &len);
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned int d = buf[i];
        sum += (seq_t)(d * d);
    }
    return sum;
}

/* grow an array of seq_t, retaining old values */
static seq_t *grow_cache(seq_t *cache, size_t old_cap, size_t new_cap)
{
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < old_cap; ++i) {
        new_cache[i] = cache[i];
    }
    free_seq_array(cache);
    return new_cache;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined, safe return */

    /* static state: cache of terms, current test value, etc. */
    static seq_t *cache = NULL;
    static size_t cnt  = 0;                 /* number of stored terms   */
    static size_t cap  = 0;                 /* allocated capacity      */
    static seq_t cur  = 1;                 /* next integer to test    */

    /* generate terms until we have at least n of them */
    while (cnt < n) {
        /* ensure enough capacity */
        if (cap < cnt + 1) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < cnt + 1) new_cap = cnt + 1;
            cache = grow_cache(cache, cap, new_cap);
            cap = new_cap;
        }

        /* test whether cur satisfies the condition */
        if (!is_prime_big(cur)) {
            seq_t ns = cur + digit_square_sum(cur);
            if (is_prime_big(ns)) {
                cache[cnt++] = cur;           /* store the term */
            }
        }
        ++cur;                                /* next integer to test */
    }

    return cache[n - 1];                     /* zero‑based index in cache */
}
