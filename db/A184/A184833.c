/*
 * A184833  a(n) = largest k such that A005117(n+1) = A005117(n) + (A005117(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A184833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = largest divisor of A005117(n) − (A005117(n+1) − A005117(n)) that
   is greater than the gap, or 0 if no such divisor exists.
   This follows from a(n) = A005117(n) − gap when that difference exceeds the
   gap; otherwise a(n) = 0. */

/* The sequence of squarefree numbers A005117 is generated on demand by
   testing each integer for squarefreeness; the helper is_squarefree
   (provided by the OEIS utilities) is used for this test. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

static seq_t *sqfree = NULL;          /* 1‑based array: sqfree[1] = 1 */
static idx_t  sqfree_capacity = 0;    /* allocated size – 1 */
static idx_t  sqfree_count   = 0;      /* number of elements stored (last index) */
static seq_t  next_candidate = 2;      /* next integer to test for square‑freeness */

/* Ensure that sqfree contains at least (target_index+1) elements,
   i.e. we know A005117(n) and A005117(n+1) for n up to target_index-1. */
static void ensure_squarefree(idx_t target_index)
{
    if (sqfree_count > target_index) return;          /* already have enough */

    /* initial allocation */
    if (sqfree == NULL) {
        sqfree_capacity = 16;
        sqfree = (seq_t *)malloc((sqfree_capacity + 1) * sizeof(seq_t));
        if (!sqfree) abort();
        sqfree[1] = (seq_t)1;              /* first square‑free number */
        sqfree_count = 1;
        next_candidate = (seq_t)2;
    }

    /* expand array if needed */
    if (sqfree_count * 2 > sqfree_capacity) {
        size_t newcap = sqfree_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(sqfree, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        sqfree = tmp;
        sqfree_capacity = newcap;
    }

    /* generate square‑free numbers up to target_index */
    while (sqfree_count <= target_index) {
        seq_t cand = next_candidate;
        while (!is_squarefree(cand)) { cand++; }
        sqfree_count++;
        if (sqfree_count >= sqfree_capacity) {
            size_t newcap = sqfree_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(sqfree, (newcap + 1) * sizeof(seq_t));
            if (!tmp) abort();
            sqfree = tmp;
            sqfree_capacity = newcap;
        }
        sqfree[sqfree_count] = cand;
        next_candidate = cand + 1;
    }
}

/* Main function computing A184833 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* offset of sequence */

    /* need A005117(n) and A005117(n+1) */
    ensure_squarefree(n + 1);

    seq_t s = sqfree[n];
    seq_t t = sqfree[n + 1];
    seq_t diff = t - s;                       /* gap */
    seq_t diff2 = s - diff;                   /* potential k */

    if (diff2 > diff) return diff2;           /* k = s - diff */
    return (seq_t)0;
}
