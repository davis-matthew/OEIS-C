/*
 * A008294  Triangle of coefficients in expansion of D^n (sec x) / sec x in
 *          powers of tan x.
 * https://oeis.org/A008294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf, nice
 * Formula:   a(n) enumerates the non‑zero entries of the triangle defined by
 *            a(0,k)=δ(0,k) and a(n+1,k)=k*a(n,k-1)+(k+1)*a(n,k+1)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 50, u64 n <= 114,
 *            u128 n <= 294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the sequence A008294. */

#include <stdlib.h>
#include <stdbool.h>

/* helper: compute value a(r, c) of the triangle */
static seq_t
triangle_value(idx_t r, idx_t c)
{
    /* row 0 */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)1;

    if (r == 0) {
        seq_t val = (c == 0) ? prev[0] : (seq_t)0;
        free(prev);
        return val;
    }

    /* iterate rows up to r */
    for (idx_t n = 1; n <= r; ++n) {
        seq_t *curr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        for (idx_t k = 0; k <= n; ++k) {
            seq_t val = (seq_t)0;
            if (k > 0)
                val += (seq_t)k * prev[k - 1];
            if (k + 1 < n)
                val += (seq_t)(k + 1) * prev[k + 1];
            curr[k] = val;
        }
        free(prev);
        prev = curr;
    }

    seq_t result = prev[c];
    free(prev);
    return result;
}

/* Main sequence function required by the test harness */
seq_t a(idx_t n)
{
    /* Determine which row and column in the triangle correspond to
       the nth non‑zero entry.  The number of non‑zero entries in row r
       is floor(r/2)+1.  We accumulate until the cumulative count
       exceeds n.  Indices are 0‑based. */
    idx_t cumulative = 0;
    idx_t r = 0;
    for (;;) {
        idx_t entries = (r / 2) + 1;       /* non‑zero entries in row r */
        if (cumulative + entries > n)
            break;
        cumulative += entries;
        ++r;
    }

    idx_t pos_in_row = n - cumulative;          /* 0‑based */
    idx_t parity = r & 1;                       /* 0 for even, 1 for odd rows */
    idx_t col = parity + (pos_in_row << 1);      /* 2*pos_in_row + parity */

    return triangle_value(r, col);
}
