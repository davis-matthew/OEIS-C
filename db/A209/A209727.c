/*
 * A209727  T(n,k) = 1/4 the number of (n+1) X (k+1) 0..2 arrays with every 2
 *          X 2 subblock having distinct clockwise edge differences.
 * https://oeis.org/A209727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 465, u32 n <= 1512, u64 n <= 1512,
 *            u128 n <= 1512, big any n
 * Helpers:   build_adjacency, compute_bases
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,1) = 2*T(n-2,1)   (T(1,1)=2, T(2,1)=3)
   For k >= 2: T(n,k) = T(n-1,k) + 2*T(n-2,k) - 2*T(n-3,k)   (T(1,k),T(2,k),T(3,k) pre‑computed)
   a(n) is the anti‑diagonal enumeration of the triangle T(n,k). */

#include <stddef.h>   /* size_t */
#include <stdbool.h>
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/build_adjacency.h"
#include "../utils/compute_bases.h"

/* Maximum number of columns we pre‑compute.  For n <= 10 000 the
   largest column needed is about 141; 200 is safely above. */
#define MAX_COLUMN 200

/* Base values T(1,k), T(2,k), T(3,k) (index 0 corresponds to k=1). */
static seq_t base1[MAX_COLUMN];
static seq_t base2[MAX_COLUMN];
static seq_t base3[MAX_COLUMN];
static unsigned int base_max_computed = 0;   /* largest k for which bases are ready */

/* --------------------------------------------------------------------- */
/* Build the adjacency matrix for a given number of rows.               */

/* --------------------------------------------------------------------- */
/* Compute base values T(1,k), T(2,k), T(3,k) up to k = maxK-1            */
/* (index 0 corresponds to k = 1).                                      */

/* --------------------------------------------------------------------- */
/* Ensure that base tables are ready up to column kNeeded.                */
/* --------------------------------------------------------------------- */
static void ensure_bases(unsigned int kNeeded)
{
    if (kNeeded <= 1 || kNeeded <= base_max_computed) return;
    if (kNeeded > MAX_COLUMN) kNeeded = MAX_COLUMN;   /* safety cap */

    compute_bases(2U, kNeeded, base1);   /* n = 1  (rows = 2) */
    compute_bases(3U, kNeeded, base2);   /* n = 2  (rows = 3) */
    compute_bases(4U, kNeeded, base3);   /* n = 3  (rows = 4) */
    base_max_computed = kNeeded;
}

/* --------------------------------------------------------------------- */
/* Return T(m,k) for arbitrary m>=1, k>=1.                                */
/* --------------------------------------------------------------------- */
static seq_t get_T(idx_t m, idx_t k)
{
    if (k == (idx_t)1) {
        /* Column 1 recurrence: T(1,1)=2, T(2,1)=3, T(n,1)=2*T(n-2,1) */
        if (m == (idx_t)1) return (seq_t)2;
        if (m == (idx_t)2) return (seq_t)3;
        seq_t prev2 = (seq_t)2;   /* T(1,1) */
        seq_t prev1 = (seq_t)3;   /* T(2,1) */
        seq_t cur;
        for (idx_t i = (idx_t)3; i <= m; ++i) {
            cur = (seq_t)2 * prev2;
            prev2 = prev1;
            prev1 = cur;
        }
        return cur;
    }

    /* k >= 2 */
    unsigned int kU = (unsigned int)k;
    if (kU > MAX_COLUMN) return (seq_t)0;   /* outside pre‑computed range */
    ensure_bases(kU);

    if (m == (idx_t)1) return base1[kU - 1];
    if (m == (idx_t)2) return base2[kU - 1];
    if (m == (idx_t)3) return base3[kU - 1];

    seq_t prev3 = base1[kU - 1];   /* T(1,k) */
    seq_t prev2 = base2[kU - 1];   /* T(2,k) */
    seq_t prev1 = base3[kU - 1];   /* T(3,k) */
    seq_t cur;
    for (idx_t i = (idx_t)4; i <= m; ++i) {
        cur = prev1 + (seq_t)2 * prev2 - (seq_t)2 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

/* --------------------------------------------------------------------- */
/* Anti‑diagonal cache for a(n).                                         */
/* --------------------------------------------------------------------- */
static seq_t *cache_a = NULL;          /* cache_a[0] unused, cache_a[i] = a(i) */
static size_t cache_size = 0;         /* number of stored entries (incl. dummy) */
static idx_t   diag = 2;              /* sum row+col of current anti‑diagonal */
static idx_t   row_in_diag = 1;       /* current row within diagonal */

/* Extend cache up to a given index. */
static void extend_cache(idx_t target)
{
    if (cache_a == NULL) {
        cache_a = (seq_t *)malloc(sizeof(seq_t));
        if (cache_a == NULL) abort();              /* allocation failure */
        cache_a[0] = (seq_t)0;                     /* dummy */
        cache_size = 1;
    }

    while (cache_size <= target) {
        idx_t r = row_in_diag;
        idx_t c = diag - row_in_diag;
        seq_t val = get_T(r, c);

        seq_t *tmp = (seq_t *)realloc(cache_a, (cache_size + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();                  /* allocation failure */
        cache_a = tmp;
        cache_a[cache_size] = val;
        ++cache_size;

        /* Advance to next position in anti‑diagonal order */
        ++row_in_diag;
        if (row_in_diag > diag - (idx_t)1) {
            ++diag;
            row_in_diag = (idx_t)1;
        }
    }
}

/* --------------------------------------------------------------------- */
/* External function required by the harness.                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for bad input */
    extend_cache(n);
    return cache_a[n];
}
