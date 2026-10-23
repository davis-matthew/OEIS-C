/*
 * A140971  Numbers k such that arithmetic mean of squares of the first k
 *          nonzero Fibonacci numbers is an integer.
 * https://oeis.org/A140971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 4042, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth positive integer k such that
   k divides F_k * F_{k+1}   (equivalently, the arithmetic mean of
   the first k Fibonacci squares is integer).  The function
   generates terms on demand, caching previously found values. */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* modular multiplication using 128‑bit arithmetic  (k < 2^64)          */
/* ------------------------------------------------------------------ */
static inline idx_t mod_mul(idx_t a, idx_t b, idx_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* ------------------------------------------------------------------ */
/* fast doubling Fibonacci modulo `mod`. Returns F_n, F_{n+1}.          */
/* ------------------------------------------------------------------ */
static void fib_mod_rec(idx_t n, idx_t mod, idx_t *fn, idx_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;      /* mod ≥ 1 in this problem */
        return;
    }

    idx_t a, b;
    fib_mod_rec(n >> 1, mod, &a, &b);      /* a=F_k   b=F_{k+1}  */

    idx_t two_b     = (b * 2) % mod;                           /* 2*F_{k+1} */
    idx_t temp      = (two_b >= a) ? (two_b - a) : (mod + two_b - a);
    idx_t c         = mod_mul(a, temp, mod);                    /* F_{2k}   */
    idx_t d         = (mod_mul(a, a, mod) + mod_mul(b, b, mod)) % mod; /* F_{2k+1} */

    if ((n & 1) == 0) {          /* n even */
        *fn  = c;
        *fn1 = d;
    } else {                      /* n odd */
        *fn  = d;
        *fn1 = (c + d >= mod) ? (c + d - mod) : (c + d);
    }
}

/* ------------------------------------------------------------------ */
/* cache for already found terms                                       */
/* ------------------------------------------------------------------ */
static seq_t *terms   = NULL;   /* array of terms (as seq_t) */
static size_t terms_len = 0;    /* number of stored terms   */
static size_t terms_cap = 0;    /* current capacity        */
static idx_t  checked   = 0;    /* largest k processed so far */

/* append a new qualifying 'k' to the cache */
static inline void append_term(idx_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = (seq_t)val;      /* cast to build's seq_t type */
}

/* ------------------------------------------------------------------ */
/* public function required by the framework                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        ++checked;                       /* new candidate k */

        idx_t f_k, f_kp1;
        fib_mod_rec(checked, checked, &f_k, &f_kp1);

        /* check divisibility: k | F_k * F_{k+1} */
        if (mod_mul(f_k, f_kp1, checked) == 0) {
            append_term(checked);
        }
    }

    return terms[n - 1];
}
