/*
 * A078345  Numbers k such that F(k) mod k divides F(F(k) mod k) where F(k)
 *          denotes the k-th Fibonacci number.
 * https://oeis.org/A078345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 9132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer k such that
   F(k) mod k divides F(F(k) mod k). */

#include <stdbool.h>

/* Fast Fibonacci modulo using fast doubling.
   All intermediate results fit into unsigned __int128. */
static inline idx_t mul_mod(idx_t a, idx_t b, idx_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* Computes (F(n), F(n+1)) modulo mod. */
static void fib_pair_mod(idx_t n, idx_t mod, idx_t *fn, idx_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    idx_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);
    idx_t two_b = (b << 1) % mod;
    idx_t c = mul_mod(a, (two_b + mod - a) % mod, mod);
    idx_t d = (mul_mod(a, a, mod) + mul_mod(b, b, mod)) % mod;
    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

static inline idx_t fib_mod_idx(idx_t n, idx_t mod) {
    idx_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t r = fib_mod_idx(k, k);
        bool ok;
        if (r == 0) {
            /* F(k) mod k == 0; F(0)=0, 0 divides 0 is taken as true. */
            ok = true;
        } else {
            ok = (fib_mod_idx(r, r) == 0);
        }
        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
