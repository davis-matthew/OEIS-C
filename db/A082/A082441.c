/*
 * A082441  Palindromic primes with middle digit 5.
 * https://oeis.org/A082441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic primes with middle digit 5
   a(n) is the n-th prime of the form
   P = H · 10^(h+1) + 5·10^h + reverse(H),
   where h = (len(P) – 1)/2 and the most significant digit of H is odd
   and not 5.  The values are generated on demand and cached. */

#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *cache      = NULL;   /* cached sequence values            */
static size_t cache_len  = 0;      /* number of values stored           */
static size_t cache_cap  = 0;      /* allocated capacity                */

/* pow10[i] = 10^i  (for i up to 20; larger values are not needed for n ≤ 10000) */
static seq_t pow10[21];
static bool pow10_inited = false;

static void init_pow10(void)
{
    if (pow10_inited) return;
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 21; ++i)
        pow10[i] = pow10[i-1] * (seq_t)10;
    pow10_inited = true;
}

/* Append a value to the cache, expanding the array if necessary */
static void add_to_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(1);
        cache   = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Reverse the decimal digits of x, assuming it has exactly h digits. */
static seq_t reverse_digits(seq_t x, int h)
{
    seq_t rev = 0;
    for (int i = 0; i < h; ++i) {
        rev = rev * (seq_t)10 + x % (seq_t)10;
        x  /= (seq_t)10;
    }
    return rev;
}

/* Cached state of the generator */
static int  gen_h      = 0;   /* current half‑length (h)           */
static seq_t gen_half  = 0;   /* current half being tested         */
static seq_t gen_end   = 0;   /* first half that is out of range   */

static void advance_generator(void)
{
    /* If we have not started, seed with the single‑digit prime 5 */
    if (gen_h == 0) {
        add_to_cache((seq_t)5);    /* a(1) = 5 */
        gen_h   = 1;               /* move to length 3 palindromes    */
        gen_half = pow10[0];       /* first 1‑digit half (1)          */
        gen_end   = pow10[1];      /* up to 10 (exclusive)            */
        return;
    }

    /* compute powers of ten for current h */
    seq_t pow10_h      = pow10[gen_h];
    seq_t pow10_hplus1 = pow10[gen_h + 1];
    seq_t pow10_hminus1= pow10[gen_h - 1];

    /* loop until we find a new prime palindrome or exhaust this h */
    while (gen_half < gen_end) {
        /* skip halves whose leading digit is even or 5 */
        seq_t first_digit = gen_half / pow10_hminus1;
        if ((first_digit & 1) == 0 || first_digit == 5) {
            ++gen_half;
            continue;
        }

        seq_t rev_half   = reverse_digits(gen_half, gen_h);
        seq_t candidate  = gen_half * pow10_hplus1
                         + (seq_t)5 * pow10_h
                         + rev_half;

        if (is_prime_big(candidate))
            add_to_cache(candidate);

        ++gen_half;
    }

    /* finished current h, move to next length */
    ++gen_h;
    gen_half = pow10[gen_h - 1];
    gen_end   = pow10[gen_h];
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_pow10();

    while (cache_len < n) {
        advance_generator();
    }
    return cache[n - 1];
}
