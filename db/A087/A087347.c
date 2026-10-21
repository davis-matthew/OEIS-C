/*
 * A087347  Numbers k that divide Sum_{j=1..k} Fibonacci(j*k).
 * https://oeis.org/A087347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that k | sum_{j=1..k} Fib(j*k)
 * Range:     u8 n <= 47, u16 n <= 184, u32 n <= 184, u64 n <= 184,
 *            u128 n <= 184, big any n
 * Helpers:   add_mod, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper types and functions */

#include <stddef.h>
#include <stdbool.h>

/* Utility for dynamic array resizing (provided by the framework) */
#include "../utils/grow_seq.h"
#include "../utils/add_mod.h"

/* 2x2 matrix with elements modulo 'mod' */
typedef struct {
    seq_t a00, a01;
    seq_t a10, a11;
} mat22_t;

/* Modular multiplication of two seq_t numbers */
static inline seq_t mul_mod(seq_t x, seq_t y, seq_t mod) {
    return (x * y) % mod;
}

/* Matrix multiplication: out = X * Y (mod 'mod') */
static inline void mat_mul(const mat22_t *x, const mat22_t *y, seq_t mod, mat22_t *out) {
    out->a00 = ( (x->a00 * y->a00) % mod + (x->a01 * y->a10) % mod ) % mod;
    out->a01 = ( (x->a00 * y->a01) % mod + (x->a01 * y->a11) % mod ) % mod;
    out->a10 = ( (x->a10 * y->a00) % mod + (x->a11 * y->a10) % mod ) % mod;
    out->a11 = ( (x->a10 * y->a01) % mod + (x->a11 * y->a11) % mod ) % mod;
}

/* Matrix addition: out = X + Y (mod 'mod') */
static inline void mat_add(const mat22_t *x, const mat22_t *y, seq_t mod, mat22_t *out) {
    out->a00 = (x->a00 + y->a00) % mod;
    out->a01 = (x->a01 + y->a01) % mod;
    out->a10 = (x->a10 + y->a10) % mod;
    out->a11 = (x->a11 + y->a11) % mod;
}

/* Fast doubling Fibonacci modulo 'mod'.
   Returns F_n in *fn, F_{n+1} in *fn1. */
static inline void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = (seq_t)1 % mod;
        return;
    }
    seq_t a, b;                     /* a = F_k , b = F_{k+1} */
    fib_pair_mod(n >> 1, mod, &a, &b);
    /* c = F_{2k} = F_k * (2*F_{k+1} - F_k) */
    seq_t t = ((b * (seq_t)2) % mod + mod - a) % mod;   /* (2*b - a) mod */
    seq_t c = (a * t) % mod;
    /* d = F_{2k+1} = F_k^2 + F_{k+1}^2 */
    seq_t d = ( (a * a) % mod + (b * b) % mod ) % mod;
    if (n & 1) {                 /* odd */
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {                     /* even */
        *fn  = c;
        *fn1 = d;
    }
}

/* Computes A^n and S = A + A^2 + ... + A^n (mod 'mod').
   Both results are returned through the pointers powOut and sumOut. */
static inline void mat_pow_sum(const mat22_t *A, idx_t n, seq_t mod,
                               mat22_t *powOut, mat22_t *sumOut) {
    if (n == 1) {
        *powOut = *A;
        *sumOut = *A;
        return;
    }
    if (n & 1) { /* odd */
        mat22_t powPrev, sumPrev;
        mat_pow_sum(A, n - 1, mod, &powPrev, &sumPrev);
        mat_mul(&powPrev, A, mod, powOut);          /* A^n = A^(n-1) * A */
        mat_add(&sumPrev, powOut, mod, sumOut);     /* Sum = previous + A^n */
    } else { /* even */
        mat22_t powHalf, sumHalf;
        mat_pow_sum(A, n >> 1, mod, &powHalf, &sumHalf);
        mat22_t powFull;
        mat_mul(&powHalf, &powHalf, mod, &powFull); /* A^n = (A^{n/2})^2 */
        *powOut = powFull;
        mat22_t temp;
        mat_mul(&powHalf, &sumHalf, mod, &temp);    /* A^{n/2} * Sum_{1..n/2} */
        mat_add(&sumHalf, &temp, mod, sumOut);      /* complete the sum */
    }
}

/* The sequence function, with memoization. */
static seq_t *seq_cache = NULL;      /* array of found k's */
static size_t cache_cap = 0;         /* capacity of seq_cache */
static idx_t seq_len  = 0;           /* number of terms stored */
static idx_t next_k = 1;             /* next integer k to test */

seq_t a(idx_t n) {
    /* Ensure at least n terms are computed. */
    while (seq_len < n) {
        idx_t k = next_k++;
        seq_t modk = (seq_t)k;

        /* Compute F_k and F_{k+1} modulo k. */
        seq_t f_k, f_k1;
        fib_pair_mod(k, modk, &f_k, &f_k1);

        /* F_{k-1} = F_{k+1} - F_k */
        seq_t f_k_minus1 = (f_k1 + modk - f_k) % modk;

        /* Build matrix A = M^k mod k.            *
         *   A = [[F_{k+1}, F_k],                   *
         *        [F_k,     F_{k-1}]] */
        mat22_t A;
        A.a00 = f_k1 % modk;
        A.a01 = f_k  % modk;
        A.a10 = f_k  % modk;
        A.a11 = f_k_minus1 % modk;

        /* Compute the sum of powers S = A + A^2 + … + A^k modulo k. */
        mat22_t powA, sumA;
        mat_pow_sum(&A, k, modk, &powA, &sumA);

        /* The required sum of Fibonacci terms is the (0,1) entry of S. */
        if (sumA.a01 == 0) {                /* k divides the sum */
            /* Store k in the cache if capacity is exceeded. */
            if (seq_len == cache_cap) {
                grow_seq(&seq_cache, &cache_cap,
                         (seq_len + 1));    /* new_len = seq_len + 1 */
            }
            seq_cache[seq_len++] = k;
        }
    }

    return seq_cache[(size_t)(n - 1)];
}
