/*
 * A068674  Numbers which yield primes when a 3 is prefixed or appended.
 * https://oeis.org/A068674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A068674 – numbers n such that both 3 ∘ n and n∘3 are prime. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Cache of found terms.  cache[0] corresponds to a(1). */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static size_t  cap    = 0;      /* capacity of cache    */
    static size_t  count  = 0;      /* number of cached terms */
    static idx_t   last_checked = 0;/* highest integer already examined */

    if (n <= count)           /* value already known */
        return cache[n - 1];

    if (!cache) {              /* initialise cache */
        cap = 128;
        cache = mem_new(seq_t, cap);
    }

    idx_t x = last_checked + 1;      /* next integer to test */

    while (count < n) {              /* generate terms until we reach n */
        seq_t xf  = (seq_t)x;               /* cast for arithmetic */
        int    d  = num_digits(xf);          /* decimal digits of x   */
        seq_t pref = (seq_t)3 * pow10((idx_t)d) + xf;
        seq_t suff = xf * (seq_t)10 + (seq_t)3;

        if (is_prime_big(pref) && is_prime_big(suff)) {
            if (count == cap) {               /* grow cache if needed */
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[count++] = xf;              /* store found term */
        }
        ++x;
    }

    last_checked = x - 1;          /* remember highest tested integer */
    return cache[n - 1];
}
