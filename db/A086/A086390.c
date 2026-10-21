/*
 * A086390  Composite n such that F(n^2+1)==1 (mod n) where F(k) denotes the
 *          k-th Fibonacci number.
 * https://oeis.org/A086390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite integer m such that F(m^2+1) ≡ 1 (mod
 *            m)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_mul.h"

/* Fast‑doubling Fibonacci modulo `mod`.  All arithmetic is performed in
 * seq_t so the routine works for both builtin‑type targets and
 * arbitrary‑precision implementations. */
static void fib_pair_mod(idx_t k, seq_t mod, seq_t *fk, seq_t *fk1)
{
    if (k == 0) {
        *fk  = (seq_t)0;
        *fk1 = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair_mod(k >> 1, mod, &a, &b);

    /* c = F(2m)   = a * (2*b – a)  (mod mod) */
    seq_t two_b   = seq_mul(b, (seq_t)2) % mod;
    seq_t temp;
    if (two_b >= a) {
        temp = two_b - a;
    } else {
        temp = two_b + mod - a;
    }
    seq_t c = seq_mul(a, temp) % mod;

    /* d = F(2m+1) = a^2 + b^2  (mod mod) */
    seq_t aa = seq_mul(a, a);
    seq_t bb = seq_mul(b, b);
    seq_t d  = (aa + bb) % mod;

    if (k & 1) {
        *fk  = d;
        *fk1 = (c + d) % mod;
    } else {
        *fk  = c;
        *fk1 = d;
    }
}

static seq_t fib_mod(idx_t k, seq_t mod)
{
    seq_t fk, fk1;
    fib_pair_mod(k, mod, &fk, &fk1);
    return fk;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static cache of found composite hits */
    static seq_t *hits   = NULL;
    static size_t  len   = 0;
    static size_t  cap   = 0;
    static uint64_t cur  = 4;          /* first candidate to test */

    if (cap == 0) {
        cap = 64;
        hits = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate hits until we have at least n of them */
    while (len < n) {
        /* test only composites */
        if (!is_prime_small((seq_t)cur)) {
            /* compute k = cur^2 + 1 (fits in 128‑bit for cur < 10^9) */
            idx_t k = (idx_t)((__uint128_t)cur * cur + 1ULL);

            seq_t fk = fib_mod(k, (seq_t)cur);
            if (fk == (seq_t)1) {
                /* store hit */
                if (len + 1 > cap) {
                    cap <<= 1;
                    hits = (seq_t *)realloc(hits, cap * sizeof(seq_t));
                }
                hits[len++] = (seq_t)cur;
            }
        }
        cur++;
    }

    return hits[n - 1];
}
