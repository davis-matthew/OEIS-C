/*
 * A068675  Numbers k such that a prime can be obtained by inserting a 1
 *          before, somewhere inside, or after the digits of k.
 * https://oeis.org/A068675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A068675: numbers k such that inserting a decimal
   digit '1' somewhere (before, between, or after the digits of k) yields
   a prime.  The routine generates the sequence by checking integers
   sequentially, caching found values for subsequent calls. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits.h"
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

static bool candidate_ok(seq_t k)
{
    int d = num_digits(k);          /* number of digits in k            */
    for (int pos = 0; pos <= d; pos++) {
        /* poww = 10^(d-pos) */
        seq_t poww = 1;
        for (int i = 0; i < d - pos; i++) {
            poww *= 10;
        }
        seq_t high = k / poww;     /* digits before insertion          */
        seq_t low  = k % poww;     /* digits after  insertion          */
        /* new number = high * 10^(d-pos+1) + 10^(d-pos) + low */
        seq_t num = high * (poww * 10) + poww + low;
        if (is_prime_big(num)) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, behaviour irrelevant */

    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static state for incremental generation */
    static seq_t next_candidate = 1;  /* next integer to test */
    static bool first_call = true;

    if (first_call) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        first_call = false;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (candidate_ok(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
