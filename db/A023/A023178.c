/*
 * A023178  Numbers k such that Fib(k) == 13 (mod k).
 * https://oeis.org/A023178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that Fib(k) == 13 (mod k)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast doubling Fibonacci with modular multiplication that never overflows. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* multiply a and b modulo mod without overflow */
static seq_t mulmod(seq_t a, seq_t b, seq_t mod) {
    seq_t res = 0;
    while (b) {
        if (b & 1) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}

/* fast doubling Fibonacci: compute F(n) and F(n+1) modulo mod */
static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn = 0;
        *fn1 = 1 % mod;
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1) */
    seq_t two_b = mulmod(b, (seq_t)2, mod);      /* 2*F(k+1) mod mod */
    seq_t temp = (two_b < a) ? (mod + two_b - a) : (two_b - a); /* (2b - a) mod mod */
    seq_t c = mulmod(a, temp, mod);             /* F(2k)    */
    seq_t d = mulmod(a, a, mod);                /* F(k)^2   */
    d += mulmod(b, b, mod);                     /* + F(k+1)^2 */
    if (d >= mod) d -= mod;
    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d);
        if (*fn1 >= mod) *fn1 -= mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* dynamic storage of found sequence values */
static seq_t   *values   = NULL;
static size_t  values_len = 0;
static size_t  values_cap = 0;
static idx_t   last_checked_k = 0;

/* ensure capacity for at least 'needed' elements */
static void ensure_values_cap(size_t needed) {
    if (needed <= values_cap) return;
    size_t new_cap = values_cap ? values_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
    values = tmp;
    values_cap = new_cap;
}

/* public function: return the nth term (offset 1) */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1, 0 is unused */
    while (values_len < (size_t)n) {
        last_checked_k++;                 /* next k to test */
        seq_t fk, fk1;
        fib_pair_mod(last_checked_k, (seq_t)last_checked_k, &fk, &fk1);
        seq_t target = 13 % last_checked_k;
        if (fk == target) {
            ensure_values_cap(values_len + 1);
            values[values_len++] = (seq_t)last_checked_k;
        }
    }
    return values[n - 1];
}
