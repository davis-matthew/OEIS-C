/*
 * A217144  Alternating sums of squares of Bell numbers (A000110).
 * https://oeis.org/A217144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*Bell(k)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static storage for Bell numbers and Stirling numbers of the second kind */
static seq_t *bell = NULL;          /* Bell numbers */
static size_t bell_cap = 0;         /* allocated capacity for bell */
static seq_t *stir = NULL;          /* current row of Stirling numbers */
static size_t stir_cap = 0;         /* allocated capacity for stir */
static idx_t cur_n = 0;             /* highest n computed */

/* Ensure that Bell numbers up to n are computed */
static void ensure_bell(idx_t n)
{
    if ((idx_t)bell_cap > n) return;          /* already sufficient */

    /* Allocate or extend the bell array */
    size_t new_bell_cap = (size_t)n + 1;
    if (bell_cap == 0) {
        bell = seq_alloc(new_bell_cap);
        bell_cap = new_bell_cap;
        bell[0] = (seq_t)1;          /* B0 = 1 */

        /* Initialise stirling numbers for n = 0 */
        stir = seq_alloc(1);
        stir_cap = 1;
        stir[0] = (seq_t)1;          /* S(0,0) = 1 */
    } else {
        bell = seq_realloc(bell, bell_cap, new_bell_cap);
        bell_cap = new_bell_cap;
    }

    /* Compute Bell numbers up to the desired n using Stirling recurrence */
    while (cur_n < n) {
        idx_t next_n = cur_n + 1;

        /* Extend the stirling array to length next_n+1 */
        stir = seq_realloc(stir, stir_cap, (size_t)next_n + 1);
        stir_cap = (size_t)next_n + 1;

        /* Update Stirling numbers in place from high k to low */
        for (size_t k = (size_t)next_n; k > 0; --k) {
            seq_t val_k    = (k - 1 <= (size_t)cur_n) ? stir[k - 1] : (seq_t)0;
            seq_t val_k1   = (k <= (size_t)cur_n)     ? stir[k]     : (seq_t)0;
            stir[k] = val_k1 * (seq_t)k + val_k;
        }
        stir[0] = (seq_t)0;   /* S(next_n,0) = 0 */

        /* Sum the new Stirling row to obtain Bell(next_n) */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= (size_t)next_n; ++k) sum += stir[k];
        bell[(size_t)next_n] = sum;

        cur_n = next_n;  /* advance current n */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_bell(n);

    seq_t pos = (seq_t)0, neg = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t = bell[(size_t)k];
        t = t * t;                   /* square Bell(k) */
        if (((n - k) & 1) == 0)
            pos += t;
        else
            neg += t;
    }
    return pos - neg;
}
