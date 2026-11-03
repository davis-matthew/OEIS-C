/*
 * A216124  Primes which are the nearest integer to the geometric mean of the
 *          previous prime and the following prime.
 * https://oeis.org/A216124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that round(sqrt(prev(p)*next(p)))
 *            == p
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 944 (2026-11-03)
 * Helpers:   ipow_i32, is_prime, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Where round is nearest integer, 0.5 rounds up) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *seq = NULL;          /* memoized terms */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* capacity of seq array */
static idx_t last_checked_prime = 1;  /* last prime processed */

/* computes the n-th term of A216124 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */

    /* one‑time allocation of the memoization array */
    if (!seq) {
        seq_cap = 64;
        seq = mem_new(seq_t, seq_cap);
        seq_len = 0;
    }

    /* generate primes until we have at least n terms */
    while (seq_len < (size_t)n) {
        idx_t p = next_prime(last_checked_prime);
        last_checked_prime = p;
        if (p <= 2)                /* skip prime 2: no previous prime */
            continue;

        idx_t prev = prev_prime(p);
        idx_t next = next_prime(p);

        /* product of consecutive primes */
        seq_t product = (seq_t)prev * (seq_t)next;

        /* compute nearest integer to sqrt(product) */
        seq_t fs = floor_sqrt(product);                 /* floor(sqrt)   */
        seq_t diff_down = product - fs * fs;            /* product - fs^2*/
        seq_t diff_up   = (fs + 1) * (fs + 1) - product;/* (fs+1)^2 - product*/
        seq_t rounded = (diff_down <= diff_up) ? fs : fs + 1;

        if (rounded == (seq_t)p) {                      /* p satisfies the condition */
            if (seq_len == seq_cap) {                    /* grow the array if needed */
                seq_cap *= 2;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
