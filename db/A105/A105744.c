/*
 * A105744  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=11.
 * https://oeis.org/A105744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal positive integer x such that * (a(n‑1))^2 +
 *            4·a(n‑2)·x is a perfect square. * Initial values: a(1) = 1, a(2)
 *            = 11
 * Range:     u8 n <= 3, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;                  /* 1‑based cache of the sequence */
static idx_t seq_cap = 0;                  /* current capacity of seq */
static idx_t seq_len = 0;                  /* next free index (<= seq_len-1 holds data) */

static inline void ensure_cap(idx_t n)
{
    /* allocate enough room to store indices up to n (inclusive) */
    if (seq_cap <= n) {
        idx_t new_cap = seq_cap ? seq_cap << 1 : 32;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but guard against 0 */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)11;

    if (seq_len == 0) {   /* first time we need the cache */
        ensure_cap(2);
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)11;
        seq_len = 3;      /* next free index */
    }

    if (n < seq_len)     /* already computed */
        return seq[n];

    /* compute terms up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        seq_t a_prev2 = seq[i - 2];          /* a(i-2) */
        seq_t a_prev1 = seq[i - 1];          /* a(i-1) */
        seq_t denom = (seq_t)4 * a_prev2;    /* 4 * a(i-2) */

        seq_t t = a_prev1 + (seq_t)1;        /* start above a_prev1 */
        seq_t x = 0;

        while (1) {
            seq_t diff = t * t - a_prev1 * a_prev1;   /* t^2 - a_prev1^2 */
            if (diff % denom == 0) {                  /* square condition */
                x = diff / denom;                     /* this is a(i) */
                break;
            }
            ++t;
        }
        ensure_cap(i);
        seq[i] = x;
    }

    seq_len = n + 1;   /* update next free index */
    return seq[n];
}
