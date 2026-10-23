/*
 * A042879  Denominators of continued fraction convergents to sqrt(971).
 * https://oeis.org/A042879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Continued‑fraction denominations for sqrt(971). Recurrence: a(n)
 *            = 24959613572660 * a(n-22) – a(n-44) for n > 43
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static cache to store computed denominators. */
static seq_t *denom = NULL;
static size_t denom_cap = 0;
static idx_t largest_computed = (idx_t)-1;

/* State for the continued fraction of sqrt(971). */
static const idx_t D = 971;      /* Radicand */
static const idx_t A0 = 31;      /* floor(sqrt(D)) */
static idx_t m = 0;              /* numerator part of the fractional remainder */
static idx_t d = 1;              /* denominator part of the fractional remainder */
static idx_t ac = A0;           /* current partial quotient (starts with a0) */

/* Previous two denominators: q_{k-2}, q_{k-1}. */
static seq_t q_prev_prev = 0;    /* q_{-1} */
static seq_t q_prev = 1;         /* q_0 */

/* Ensure that denom[n] has been computed. */
static void ensure_denoms(idx_t n)
{
    /* Allocate initial storage if necessary. */
    if (denom == NULL) {
        denom_cap = 64;
        denom = (seq_t *)realloc(denom, denom_cap * sizeof(seq_t));
    }

    /* Grow storage to accommodate index n */
    if ((size_t)n + 1 > denom_cap) {
        size_t new_cap = denom_cap;
        if (new_cap == 0) new_cap = 64;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        denom = (seq_t *)realloc(denom, new_cap * sizeof(seq_t));
        denom_cap = new_cap;
    }

    /* If nothing has been computed yet, initialise the first term. */
    if (largest_computed == (idx_t)-1) {
        denom[0] = (seq_t)1;          /* q_0 = 1 */
        largest_computed = 0;
        /* State m,d,ac already at their start values. */
    }

    /* Compute missing terms up to n. */
    for (idx_t k = largest_computed + 1; k <= n; ++k) {
        /* Generate next partial quotient a_k (ac). */
        m = d * ac - m;
        d = (D - m * m) / d;
        ac = (A0 + m) / d;

        /* Denominator recurrence: q_k = a_k * q_{k-1} + q_{k-2}. */
        seq_t q_cur = (seq_t)ac * q_prev + q_prev_prev;
        denom[k] = q_cur;

        /* Shift the last two denominators for the next iteration. */
        q_prev_prev = q_prev;
        q_prev = q_cur;
    }

    largest_computed = n;
}

/* Sequence entry function. */
seq_t a(idx_t n)
{
    ensure_denoms(n);
    return denom[n];
}
