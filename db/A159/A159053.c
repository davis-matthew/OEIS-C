/*
 * A159053  Indices n such that Fibonacci(n-3) is a multiple of n.
 * https://oeis.org/A159053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 636, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 62 terms through n = 4673 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159053: indices n such that Fibonacci(n-3) is a multiple of n */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Fast doubling Fibonacci modulo mod. fn = F(n) mod mod, fn1 = F(n+1) mod mod */
static void fib_doubling_uint64(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    uint64_t a, b;
    fib_doubling_uint64(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1) */
    uint64_t two_b = (b << 1) % mod;
    uint64_t c = (a * ((two_b + mod - a) % mod)) % mod;  /* F(2k)    */
    uint64_t d = (a * a % mod + b * b % mod) % mod;       /* F(2k+1)  */
    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* Return Fibonacci(k) modulo mod, where k may be negative. */
static uint64_t fib_mod_idx_t(int64_t k, uint64_t mod) {
    if (mod == 1) return 0;                       /* everything ≡0 (mod 1) */
    uint64_t val;
    if (k >= 0) {
        uint64_t fn, _tmp;
        fib_doubling_uint64((uint64_t)k, mod, &fn, & _tmp);
        val = fn % mod;
    } else {
        uint64_t kn = (uint64_t)(-k);
        uint64_t fn, _tmp;
        fib_doubling_uint64(kn, mod, &fn, & _tmp);
        /* F(-n) = (-1)^{n+1} F(n) */
        if ((kn + 1) & 1) {
            val = (mod - (fn % mod)) % mod;        /* negative sign */
        } else {
            val = fn % mod;
        }
    }
    return val;
}

/* Main sequence function.  seq_t and idx_t are defined by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    static seq_t *terms = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t current = 1;      /* next candidate to test */

    while (len < n) {
        uint64_t cand = (uint64_t)current;
        bool holds = false;
        /* cand is always > 0 because current starts at 1 */
        /* F(-2) ≡ 0 (mod 1) so n=1 is a solution */
        if (cand == 1) {
            holds = true;
        } else {
            int64_t k = (int64_t)cand - 3;
            uint64_t r = fib_mod_idx_t(k, cand);
            holds = (r % cand) == 0;
        }

        if (holds) {
            if (len + 1 > cap) {
                size_t newcap = cap ? cap << 1 : 8;
                seq_t *tmp = mem_resize(seq_t, terms, newcap);
                if (!tmp) {
                    /* allocation failed – abort with zero value */
                    return 0;
                }
                terms = tmp;
                cap   = newcap;
            }
            terms[len++] = (seq_t)current;
        }
        current++;
    }
    return terms[n - 1];
}
