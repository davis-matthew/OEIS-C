/*
 * A104498  Expansion of (1/2)*(1-sqrt(1-8*x)/sqrt(1-4*x)).
 * https://oeis.org/A104498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; for n ≥ 1 * a(n) = 4^{n−1} + Σ_{k=1}^{n−1} a(k)·a(n−k)
 *            (OEIS A104498)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* static storage for the sequence and powers of four */
static seq_t *seq = NULL;         /* sequence values a(n)              */
static seq_t *pow4 = NULL;        /* powers of four: pow4[i] = 4^i      */
static idx_t capacity = 0;        /* allocated capacity                 */
static idx_t size = 0;            /* number of values currently filled  */

/* grow the storage so that indices up to n are available */
static void ensure_capacity(idx_t n)
{
    if (capacity == 0) {
        /* initial capacity: enough for indices 0 and 1 */
        capacity = 4;
        seq = (seq_t *)malloc(sizeof(seq_t) * capacity);
        pow4 = (seq_t *)malloc(sizeof(seq_t) * capacity);
        seq[0] = (seq_t)0;
        pow4[0] = (seq_t)1;
        size = 1;                   /* we have computed a[0] */
    }

    if (n + 1 <= size) return;      /* already have enough */

    /* enlarge capacity if necessary */
    if (n + 1 > capacity) {
        idx_t newcap = capacity * 2;
        if (newcap < n + 1) newcap = n + 1;
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * newcap);
        pow4 = (seq_t *)realloc(pow4, sizeof(seq_t) * newcap);
        capacity = newcap;
    }

    /* compute new entries sequentially */
    for (idx_t i = size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) sum += seq[k] * seq[i - k];
        seq[i] = sum + pow4[i - 1];        /* 4^{i-1} is pow4[i-1]          */
        pow4[i] = pow4[i - 1] * (seq_t)4;   /* update 4^i for future use */
    }
    size = n + 1;                          /* we now have a[0 .. n]          */
}

/* external function that returns a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_capacity(n);
    return seq[n];
}
