/*
 * A143656  Triangle T(n, k) = A045545(k) if gcd(n,k) = 1, 0 otherwise, read
 *          by rows.
 * https://oeis.org/A143656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 251, u32 n <= 901, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   gcd, linear_to_rowcol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A143656 – flattened triangle of A045545.
 *
 *  For n ≥ 1 let (r, k) be the row and column of the n‑th element
 *  of the triangular array read row by row.  Then
 *      a(n) = T(r, k)
 *  where
 *      T(r, k) = A045545(k)  if gcd(r, k) = 1
 *                0            otherwise.
 *  The auxiliary sequence A045545 is generated on demand by the
 *  relation
 *      A045545(1) = 1
 *      A045545(m) = Σ_{1 ≤ j < m , gcd(m, j)=1} A045545(j)
 *  for m > 1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */
#include "../utils/linear_to_rowcol.h"

static seq_t *a_tbl = NULL;      /* a_tbl[1..max] holds A045545 values    */
static idx_t  a_tbl_size = 0;    /* highest index currently computed     */

/* Ensure that a_tbl contains A045545 values up to k_max.  We rebuild
   the array when necessary, keeping each element computed exactly once. */
static void ensure_a_up_to(idx_t k_max)
{
    if (k_max == 0) return;

    if (a_tbl == NULL) {
        /* Allocate space for indices 0..k_max (index 0 unused). */
        a_tbl = (seq_t *)realloc(NULL, (size_t)(k_max + 1) * sizeof(seq_t));
        if (a_tbl == NULL) return;             /* allocation failure: abort silently */
        a_tbl[1] = (seq_t)1;                   /* A045545(1) = 1 */
        a_tbl_size = 1;
    } else if (a_tbl_size >= k_max) {
        return;                                 /* already have all needed data */
    } else {
        /* Extend the existing table */
        seq_t *tmp = (seq_t *)realloc(a_tbl, (size_t)(k_max + 1) * sizeof(seq_t));
        if (tmp == NULL) return;                /* allocation failure: abort silently */
        a_tbl = tmp;
    }

    /* Compute missing entries A045545(i) for i = a_tbl_size+1 .. k_max */
    for (idx_t i = a_tbl_size + 1; i <= k_max; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < i; ++j) {
            if (gcd((seq_t)i, (seq_t)j) == (seq_t)1) {
                sum += a_tbl[(size_t)j];
            }
        }
        a_tbl[(size_t)i] = sum;
    }
    a_tbl_size = k_max;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined case, return 0 */

    idx_t r, k;
    linear_to_rowcol(n, &r, &k);

    /* If columns are not coprime, the triangle entry is 0. */
    if (gcd((seq_t)r, (seq_t)k) != (seq_t)1) {
        return (seq_t)0;
    }

    /* Ensure the auxiliary sequence holds A045545(k). */
    ensure_a_up_to(k);
    return a_tbl[(size_t)k];
}
