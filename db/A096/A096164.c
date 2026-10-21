/*
 * A096164  Triangle T(n,m) read by rows: matrix product A053121 * A038207.
 * https://oeis.org/A096164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096164 – triangle T(n,m) = A053121 * A038207, read row‑by‑row.
   a(n) is the n-th entry in the flattened triangle (offset 1).
   The triangle satisfies the recurrence
     T(n,m) = T(n-1,m-1) + 2*T(n-1,m) + Σ_{p=m+1}^{n-1} T(n-1,p)*(-2)^{p-(m+1)}
   for 1 ≤ m ≤ n, with T(1,1)=1.  Values are computed lazily,
   stored in rows 1‥r where r is the smallest integer with r(r+1)/2 ≥ n.
   The routine works for any unsigned seq_t type (including arbitrary‑precision
   integers) and any uint64_t idx_t.  The running time for n≤10000 is O(n)
   (the triangle has at most ≈140 rows).  No external libraries are used
   other than <stdlib.h>.  The code is valid C11 and C++17. */

#include <stdlib.h>

static seq_t **T = NULL;   /* triangle rows, 1‑based indexing */
static idx_t maxRow = 0;   /* number of rows already built */

/* Ensure that rows up to 'r' are computed. */
static void ensure_row(idx_t r)
{
    if (r <= maxRow)
        return;

    /* enlarge the table of row pointers */
    T = (seq_t **)realloc(T, sizeof(seq_t *) * (r + 1));
    /* build missing rows */
    for (idx_t n = maxRow + 1; n <= r; ++n) {
        T[n] = (seq_t *)malloc(sizeof(seq_t) * (n + 1)); /* T[n][0] unused */

        if (n == 1) {          /* base case */
            T[1][1] = (seq_t)1;
            continue;
        }

        seq_t *prev = T[n - 1];
        for (idx_t m = 1; m <= n; ++m) {
            seq_t val = 0;

            /* T(n-1, m-1) */
            if (m > 1)
                val += prev[m - 1];

            /* 2*T(n-1, m) */
            if (m <= n - 1)
                val += (seq_t)2 * prev[m];

            /* Σ term with alternating signs */
            seq_t sum_even = 0;
            seq_t sum_odd  = 0;
            seq_t power    = 1;           /* 2^0 */
            idx_t exp = 0;                /* exponent (p-(m+1)) */

            for (idx_t p = m + 1; p <= n - 1; ++p) {
                if ((exp & 1) == 0)          /* even exponent */
                    sum_even += prev[p] * power;
                else
                    sum_odd  += prev[p] * power;
                power *= (seq_t)2;
                ++exp;
            }

            val += sum_even - sum_odd;
            T[n][m] = val;
        }
    }
    maxRow = r;
}

/* Return the n-th term of A096164 (offset 1). */
seq_t a(idx_t n)
{
    /* Find the row containing position n. */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Column (1‑based) inside that row. */
    idx_t col = n - (r - 1) * r / 2;

    ensure_row(r);
    return T[r][col];
}
