/*
 * A126630  a(1)=1. a(n) = the number of earlier terms that are coprime to the
 *          n-th Fibonacci number.
 * https://oeis.org/A126630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126630
   a(1) = 1
   a(n) = the number of earlier terms a(k) (k < n) that are coprime to the
   n‑th Fibonacci number F(n).  (Offset 1)
   The function is memoised; it works for any order of calls.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* ---------- utilities ---------- */

/* Compute F(n) modulo mod (mod > 0) using fast doubling.
   All arithmetic fits in 64‑bit because mod ≤ 10⁴ for our use. */
static inline uint64_t fib_mod_uint64(idx_t n, uint64_t mod) {
    if (mod == 1) return 0;           /* 0 ≤ n, F(n) mod 1 = 0 */
    uint64_t a = 0; /* F(0) mod mod */
    uint64_t b = 1; /* F(1) mod mod */

    /* find index of most significant bit of n */
    int msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        tmp >>= 1;
        msb++;
    }

    for (int i = msb; i >= 0; --i) {
        /* fast‑doubling formulas */
        uint64_t two_b = (b << 1) % mod;
        uint64_t t = (two_b + mod - a) % mod;    /* (2*b - a) mod mod */
        uint64_t c = (a * t) % mod;              /* F(2k)   */
        uint64_t d = (a * a % mod + b * b % mod) % mod; /* F(2k+1) */
        if ((n >> i) & 1) { /* odd */
            a = d;
            b = (c + d) % mod;
        } else {            /* even */
            a = c;
            b = d;
        }
    }
    return a; /* F(n) mod mod */
}

/* ---------- memoisation data ---------- */
static idx_t Nmax = 0;   /* largest n already computed */
static size_t a_cap = 0;
static seq_t *a_cache = NULL;   /* array of a(1)…a(n) (1‑based index) */

/* Ensure cache large enough and compute missing terms up to n. */
static void ensure_cache(idx_t n) {
    if (n <= Nmax) return;

    /* expand capacity if needed */
    if (a_cap < (size_t)n) {
        size_t new_cap = a_cap ? a_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* compute new terms */
    for (idx_t i = Nmax + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[0] = (seq_t)1;   /* a(1) */
        } else {
            idx_t count = 0;
            for (idx_t k = 1; k < i; ++k) {
                uint64_t m = (uint64_t)a_cache[k - 1];   /* a(k) <= 10⁴ */
                uint64_t rem = fib_mod_uint64(i, m);     /* F(i) mod m */
                if (gcd_uint64(m, rem) == 1) ++count;
            }
            a_cache[i - 1] = (seq_t)count;   /* a(i) */
        }
    }
    Nmax = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    ensure_cache(n);
    return a_cache[n - 1];
}
