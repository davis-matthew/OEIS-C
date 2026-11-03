/*
 * A132791  Numbers k such that the sum of the digits of 4^k is prime.
 * https://oeis.org/A132791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, less, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132791 : Numbers k such that digit sum of 4^k is prime
 *  Implementation: iteratively build 4^k in base‑10 as an array of
 *  decimal digits.  For each new k the digit sum is computed,
 *  tested for primality (using is_prime_big), and k is cached
 *  if the digit sum is prime.  The function remembers all cached
 *  k values and the current power so that subsequent calls reuse
 *  the data and work correctly regardless of call order.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;           /* cached k values */
static size_t cache_cap = 0;          /* allocated cache size */
static idx_t   cache_cnt = 0;         /* number of cached values */

static unsigned char *digits = NULL;  /* base‑10 digits of 4^k (least‑significant first) */
static size_t digits_len = 0;         /* number of used digits */
static size_t digits_cap = 0;         /* allocated capacity (bytes) */

static idx_t next_k = 0;              /* exponent whose power is stored in 'digits' */
static bool  initialized = false;

/* Ensure that 'cache' has space for at least 'needed' entries. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure that 'digits' has space for at least 'needed' bytes. */
static void ensure_digits_capacity(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    unsigned char *new_digits = (unsigned char *)realloc(digits, new_cap);
    if (!new_digits) exit(EXIT_FAILURE);
    digits = new_digits;
    digits_cap = new_cap;
}

/* Initialise all static structures.  Sets 4^0 = 1. */
static void init_static(void)
{
    /* digits array for 4^0 */
    ensure_digits_capacity(16);
    digits[0] = 1;
    digits_len = 1;
    next_k = 0;

    /* empty cache */
    cache = NULL;
    cache_cap = 0;
    cache_cnt = 0;

    initialized = true;
}

/* Multiply the decimal number stored in 'digits' by 4. */
static void multiply_by_4(void)
{
    unsigned int carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        unsigned int tmp = (unsigned int)digits[i] * 4 + carry;
        digits[i] = (unsigned char)(tmp % 10);
        carry = tmp / 10;
    }
    while (carry > 0) {
        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = (unsigned char)(carry % 10);
        carry /= 10;
    }
}

/* Compute the sum of decimal digits stored in 'digits'. */
static seq_t digit_sum_of_current(void)
{
    seq_t sum = 0;
    for (size_t i = 0; i < digits_len; ++i) sum += digits[i];
    return sum;
}

/* Return the n-th element of the sequence.  a(1) = first term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Not defined, return 0 for safety */

    if (!initialized) init_static();

    /* Grow cache until we have n values.  Keep generating powers of 4. */
    while ((idx_t)cache_cnt < n) {
        /* Advance to next exponent.  next_k already holds the current exponent. */
        ++next_k;                    /* now represents the new exponent */

        /* Compute 4^next_k from 4^(next_k-1). */
        multiply_by_4();

        /* Test digit sum for primality. */
        seq_t dsum = digit_sum_of_current();
        if (is_prime_big(dsum)) {
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt++] = next_k;
        }
    }

    return cache[n - 1];
}
