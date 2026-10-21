/*
 * A211228  Shallow diagonal sums of A211226.
 * https://oeis.org/A211228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=4, a(6)=5, a(7)=8 *
 *            Recurrence for n ≥ 8: a(n) = 2*a(n‑2) + a(n‑4) – 2*a(n‑6) –
 *            a(n‑8)
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* for fixed-width types, if needed */

/* Forward declaration required by the build */
extern seq_t a(idx_t n);   /* just to satisfy the compiler, will be defined below */

/* --- Static memoisation state --- */
static seq_t *cache = NULL;   /* array holding a(0) … a(size‑1) */
static size_t  cap   = 0;     /* allocated array capacity (in elements) */
static idx_t   size  = 0;     /* next unused index (current number of terms) */

/* Ensure that cache holds all terms up to index n (inclusive). */
static void ensure(idx_t n)
{
    if (n < size) return;          /* already computed */

    /* Grow the buffer if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap : 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);         /* out‑of‑memory; abort */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first eight terms (only the first time) */
    if (size == 0) {
        /* guarantee space for at least 8 elements */
        if (cap < 8) {
            seq_t *tmp = (seq_t*)realloc(cache, 8 * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap = 8;
        }
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)1;  /* a(1) */
        cache[2] = (seq_t)2;  /* a(2) */
        cache[3] = (seq_t)2;  /* a(3) */
        cache[4] = (seq_t)3;  /* a(4) */
        cache[5] = (seq_t)4;  /* a(5) */
        cache[6] = (seq_t)5;  /* a(6) */
        cache[7] = (seq_t)8;  /* a(7) */
        size    = 8;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 8) {
            /* base terms are already set */
            continue;
        }
        seq_t val = cache[i-2];         /* a(i‑2) */
        val += cache[i-2];              /* 2*a(i‑2) */
        val += cache[i-4];              /* + a(i‑4) */

        seq_t tmp = cache[i-6];         /* a(i‑6) */
        tmp += tmp;                     /* 2*a(i‑6) */
        val -= tmp;                     /* – 2*a(i‑6) */

        val -= cache[i-8];              /* – a(i‑8) */

        cache[i] = val;                 /* store the result */
    }

    size = n + 1;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    ensure(n);
    return cache[n];
}
