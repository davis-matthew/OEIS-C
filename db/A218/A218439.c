/*
 * A218439  a(n) = A001609(n)^2, where g.f. of A001609 is
 *          x*(1+3*x^2)/(1-x-x^3).
 * https://oeis.org/A218439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A001609(n))^2 where A001609 satisfies: A001609(1)=1,
 *            A001609(2)=1, A001609(3)=4, A001609(n)=A001609(n-1)+A001609(n-3)
 *            for n≥4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   ensure_capacity, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/seq_mul.h"

static seq_t *gA1k = NULL;          /* cached A001609 values (1-indexed, stored 0‑based) */
static size_t gA1k_cap = 0;          /* allocated capacity           */
static size_t gA1k_computed = 0;     /* highest index computed (0‑based) */

static void _ensure_A001609(idx_t n)
{
    if (n <= (idx_t)gA1k_computed) return;

    /* Ensure we have enough space for all needed indices. */
    size_t needed_cap = (size_t)(n > 5 ? n : 5);
    ensure_capacity(needed_cap, &gA1k_cap, (void **)&gA1k, sizeof(seq_t));

    /* Initialise the first three terms if not already done. */
    if (gA1k_computed < 3) {
        if (gA1k_cap > 0) gA1k[0] = 1;          /* A001609(1) */
        if (gA1k_cap > 1) gA1k[1] = 1;          /* A001609(2) */
        if (gA1k_cap > 2) gA1k[2] = 4;          /* A001609(3) */
        gA1k_computed = 3;
    }

    /* Compute all remaining entries up to n-1 (0‑based indices). */
    for (idx_t i = gA1k_computed; i < n; ++i) {
        /* For 1‑based index k = i+1: A001609(k) = A001609(k-1) + A001609(k-3).
           In 0‑based array: gA1k[i] = gA1k[i-1] + gA1k[i-3].            */
        gA1k[i] = gA1k[i-1] + gA1k[i-3];
    }
    gA1k_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* Undefined for n<1, guard */
    _ensure_A001609(n);
    seq_t val = gA1k[n-1];              /* A001609(n) */
    return seq_mul(val, val);           /* square */
}
