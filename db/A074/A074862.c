/*
 * A074862  Iccanartet sequence: a(n)=R[a(n-1)]+R[a(n-2)]+R[a(n-3)]+R[a(n-4)]
 *          where a(1)=a(2)=a(3)=a(4)=1 and R(n) (A004086) is the reverse of
 *          n.
 * https://oeis.org/A074862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = R[a(n‑1)] + R[a(n‑2)] + R[a(n‑3)] + R[a(n‑4)], with
 *            a(1)=a(2)=a(3)=a(4)=1. R(x) is the decimal reversal of x
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 28, u64 n <= 53, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Reverse the decimal representation of a seq_t number. */
static seq_t reverse_num(seq_t n)
{
    seq_t rev = 0;
    while (n != 0) {
        seq_t d = n % 10;
        rev = rev * 10 + d;
        n = n / 10;
    }
    return rev;
}

/* Cache for computed values of the sequence. */
static seq_t *seq_cache = NULL;
static size_t  seq_cap   = 0;   /* allocated capacity */
static size_t  seq_len   = 0;   /* number of values stored (>= seq_len == computed count) */

/* Ensure the cache holds at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/*  Main function: returns a(n).  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is undefined for n=0; 0 is a safe fall‑back */
        return (seq_t)0;

    /* Initialise the cache with the first four terms if this is the first call. */
    if (seq_cache == NULL) {
        ensure_capacity(4);
        seq_cache[0] = seq_cache[1] = seq_cache[2] = seq_cache[3] = (seq_t)1;
        seq_len = 4;                 /* number of values currently stored */
    }

    /* Compute any missing values up to the requested index. */
    if (seq_len < (size_t)n) {
        ensure_capacity((size_t)n);
        for (idx_t i = (idx_t)seq_len + 1; i <= n; ++i) {   /* i is 1‑based */
            seq_t next;
            if (i <= 4)           /* base case handled by initialisation already */
                next = (seq_t)1;
            else {
                seq_t s = 0;
                /* indices for a(i‑1), a(i‑2), a(i‑3), a(i‑4) in 0‑based array are
                   i‑2, i‑3, i‑4, i‑5 respectively.  All of these exist because
                   seq_len >= i‑1 before the step starts. */
                s += reverse_num(seq_cache[i-2]);   /* a(i‑1)  */
                s += reverse_num(seq_cache[i-3]);   /* a(i‑2)  */
                s += reverse_num(seq_cache[i-4]);   /* a(i‑3)  */
                s += reverse_num(seq_cache[i-5]);   /* a(i‑4)  */
                next = s;
            }
            seq_cache[i-1] = next;
        }
        seq_len = (size_t)n;
    }

    return seq_cache[n-1];
}
