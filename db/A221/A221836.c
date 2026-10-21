/*
 * A221836  Triangle in which m-th term of n-th row is the number of integer
 *          Heron triangles with two of the sides having lengths n, m.
 * https://oeis.org/A221836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of integer Heron triangles with two sides of
 *            lengths r and c, where (r, c) are the row and column of the
 *            flattened triangular array. The triangle is indexed as
 *            a(1)=a(1,1), a(2)=a(2,1), a(3)=a(2,2), a(4)=a(3,1), …
 * Range:     u8 n <= 13, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  To compute a given pair (r,c) we brute‑force the third side k.
    For each integer k in the open interval |r-c| < k < r+c we test
    whether the Heron's formula yields a perfect square.  This is
    efficient for the problem size – the largest row needed for
    n≤10000 is 141, so at most 280 candidates per pair.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Helper: count integer Heron triangles with two fixed sides a, b. */
static seq_t count_heron(idx_t a, idx_t b)
{
    idx_t min_k = (a > b) ? (a - b) : (b - a);
    min_k += 1;                     /* strict inequality */
    idx_t max_k = a + b - 1;        /* strict inequality */

    /* s = (a+b+k)/2 must be integer, so k must have same parity as a+b. */
    if (((min_k + a + b) & 1) != 0)
        ++min_k;                     /* adjust to correct parity */
    seq_t cnt = 0;
    for (idx_t k = min_k; k <= max_k; k += 2) {
        seq_t s = ((seq_t)a + (seq_t)b + (seq_t)k) >> 1;   /* (a+b+k)/2 */

        /* Compute s*(s-a)*(s-b)*(s-k) using seq_t to avoid overflow. */
        seq_t val = s * (s - (seq_t)a) * (s - (seq_t)b) * (s - (seq_t)k);
        if (val < 0)
            continue;                /* should not happen */
        seq_t t = floor_sqrt(val);
        if (t * t == val)
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n, and column c = n - r(r-1)/2. */
    idx_t r = 1;
    for (;;) {
        idx_t tri = r * (r + 1) / 2;
        if (tri >= n)
            break;
        ++r;
    }
    idx_t c = n - r * (r - 1) / 2;

    /* Count Heron triangles with sides r and c. */
    return count_heron(r, c);
}
