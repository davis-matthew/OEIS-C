/*
 * A145579  Eigentriangle, row sums = A001590.
 * https://oeis.org/A145579
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = diag(c) * ( ((r - c) mod 3) != 2 ), where (r,c) are the
 *            row and column indices of the n‑th triangle element, and
 *            diag(1)=1, diag(c)=A001590(c+1) for c>1. A001590 is defined by
 *            a(0)=0, a(1)=1, a(2)=0, a(k)=a(k-1)+a(k-2)+a(k-3) for k≥3
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Max column needed for n up to 10000 is about 141.
   A few extra values are precomputed for safety. */
#define MAX_A 200

static seq_t A_values[MAX_A + 1];
static int A_initialized = 0;

static void init_A_values(void)
{
    if (A_initialized) return;
    A_values[0] = 0;
    A_values[1] = 1;
    A_values[2] = 0;
    for (idx_t i = 3; i <= MAX_A; ++i) {
        A_values[i] = A_values[i-1] + A_values[i-2] + A_values[i-3];
    }
    A_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    init_A_values();

    idx_t k = n - 3;                /* zero‑based position in flattened triangle */

    /* find row r such that r*(r+1)/2 > k */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 <= k) ++r;

    /* total elements in previous rows */
    idx_t prev = (r * (r - 1)) / 2;
    /* column index (1‑based) */
    idx_t c = k - prev + 1;

    /* pattern (1,1,0) repeats every 3 rows in each column */
    idx_t diff   = r - c;
    idx_t pat    = diff % 3;   /* 0->1, 1->1, 2->0 */
    seq_t pat_val = (pat == 2) ? 0 : 1;

    /* diagonal value */
    seq_t diag;
    if (c == 1)
        diag = 1;
    else
        diag = A_values[c + 1];   /* A001590(c+1) */

    return diag * pat_val;
}
