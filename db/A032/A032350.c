/*
 * A032350  Palindromic nonprime numbers.
 * https://oeis.org/A032350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 683, u32 n <= 2005, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   is_prime_big, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes palindromic nonprime numbers (complement of palindromic primes) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Helper: integer power of ten for 64‑bit values. */
static uint64_t pow10_u64(uint64_t e)
{
    uint64_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* Build a palindrome from a numeric prefix.  If odd is true, the
   middle digit is not repeated. */
static seq_t make_pal(uint64_t prefix, bool odd)
{
    seq_t pal = (seq_t)prefix;
    uint64_t rev = prefix;
    if (odd) rev /= 10;
    while (rev > 0) {
        pal = pal * (seq_t)10 + (seq_t)(rev % 10);
        rev /= 10;
    }
    return pal;
}

/* ------------------------------------------------------------------ */
/* Incremental palindromic number generator */
static bool __pal_gen_inited = false;
static uint64_t __pal_gen_len   = 1;         /* current digit length   */
static bool       __pal_gen_odd  = true;      /* length odd/even flag   */
static uint64_t   __pal_gen_start=1;         /* first prefix for length*/
static uint64_t   __pal_gen_end  =9;         /* last  prefix for length*/
static uint64_t   __pal_gen_prefix=1;        /* next prefix to produce*/

/* Initialize the generator once. */
static void __pal_gen_init(void)
{
    __pal_gen_inited = true;
    __pal_gen_len    = 1;
    __pal_gen_odd    = true;
    uint64_t half = (__pal_gen_len + 1) / 2;
    __pal_gen_start = pow10_u64(half - 1);
    __pal_gen_end   = pow10_u64(half) - 1;
    __pal_gen_prefix = __pal_gen_start;
}

/* Return the next palindromic number in ascending order. */
static seq_t __pal_gen_next(void)
{
    if (!__pal_gen_inited) __pal_gen_init();

    seq_t pal = make_pal(__pal_gen_prefix, __pal_gen_odd);

    /* Advance prefix for next call. */
    if (__pal_gen_prefix < __pal_gen_end) {
        __pal_gen_prefix++;
    } else {
        /* Move to the next length. */
        __pal_gen_len++;
        __pal_gen_odd = (__pal_gen_len % 2 == 1);
        uint64_t half = (__pal_gen_len + 1) / 2;
        __pal_gen_start = pow10_u64(half - 1);
        __pal_gen_end   = pow10_u64(half) - 1;
        __pal_gen_prefix = __pal_gen_start;
    }
    return pal;
}

/* ------------------------------------------------------------------ */
/* Cache for computed terms.  The cache grows as needed. */
static seq_t *   __cache = NULL;
static size_t    __cache_len = 0;
static size_t    __cache_cap = 0;

/* Grow the cache to at least 'needed' entries. */
static void __ensure_cache(size_t needed)
{
    if (needed <= __cache_cap) return;

    size_t new_cap = __cache_cap ? __cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    /* Copy existing data. */
    for (size_t i = 0; i < __cache_len; i++) {
        new_arr[i] = __cache[i];
    }
    /* Free old array if it existed. */
    if (__cache != NULL) {
        seq_free_array(__cache);
    }
    __cache      = new_arr;
    __cache_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* The required sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n==0 – return 0 as a safe default. */
    if (n == 0) return 0;

    /* If we already have the answer, return it. */
    if ((size_t)n <= __cache_len) {
        return __cache[(size_t)n - 1];
    }

    size_t target = (size_t)n;
    __ensure_cache(target);

    /* Generate palindromes until we have enough nonprime ones. */
    while (__cache_len < target) {
        seq_t pal = __pal_gen_next();

        /* 1 is not prime; other numbers are tested by is_prime_big. */
        if (pal == (seq_t)1 || !is_prime_big(pal)) {
            __cache[__cache_len++] = pal;
        }
    }

    return __cache[target - 1];
}
