/*
 * A160684  Numbers k >= 1 such that A000045(k)/A000005(k) is an integer.
 * https://oeis.org/A160684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 6055, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th integer k ≥ 1 for which the Fibonacci
 * number F_k is divisible by d(k), the divisor count of k.
 *
 * Algorithm:  increment k, test the congruence
 *     fib(k) mod d(k) == 0
 * until the desired n‑th term is found.  Results are cached so
 * repeated calls are fast.  The implementation uses the supplied
 * utility function divisor_count_u64() for d(k) and a fast‑doubling
 * routine for Fibonacci modulo a small modulus.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   /* provides uint64_t divisor_count_u64(uint64_t) */

/* ---------- fast doubling Fibonacci modulo mod --------------------------------- */
static inline void fib_pair_mod(idx_t n, uint64_t mod,
                                uint64_t *f, uint64_t *g)
{
    if (n == 0) {
        *f = 0;
        *g = 1;
        return;
    }
    uint64_t a, b, c, d;
    fib_pair_mod(n >> 1, mod, &a, &b);      /* a = F(k), b = F(k+1) */
    /* F(2k) = F(k) * (2*F(k+1) – F(k)) */
    uint64_t two_b = (b << 1) % mod;        /* 2*F(k+1) mod mod */
    c = (a * ((two_b + mod - a) % mod)) % mod; /* F(2k) mod mod */
    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    d = (a * a + b * b) % mod;              /* F(2k+1) mod mod */
    if ((n & 1) == 0) {
        *f = c;   *g = d;
    } else {
        *f = d;   *g = (c + d) % mod;
    }
}

static inline uint64_t fib_mod(idx_t n, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t f, g;
    fib_pair_mod(n, mod, &f, &g);
    return f;
}

/* ---------- cached sequence storage ------------------------------------------- */
static seq_t *seq_vals = NULL;   /* pointer to array of stored terms   */
static size_t seq_len = 0;       /* number of terms stored            */
static size_t seq_cap = 0;       /* allocated capacity                */
static uint64_t last_k = 0;      /* largest k examined so far         */

static void grow_seq(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_vals = tmp;
    seq_cap = new_cap;
}

/* ---------- main access function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is not defined for this sequence */

    if (!seq_vals) grow_seq(64);   /* first call: allocate storage            */

    while (seq_len < (size_t)n) {  /* generate terms until we have n entries   */
        ++last_k;                  /* next k to test                           */
        uint64_t d = divisor_count_u64(last_k);
        if (fib_mod(last_k, d) == 0) {
            grow_seq(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)last_k;
        }
    }

    return seq_vals[(size_t)n - 1];
}
