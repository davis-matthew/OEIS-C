/*
 * A117396  Triangle, read by rows, defined by: T(n,k) = (k+1)*T(n,k+1) -
 *          Sum_{j=1..n-k-1} T(j,0)*T(n,j+k+1) for n>k with T(n,n)=1 for n>=0.
 * https://oeis.org/A117396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 105, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A117396 – Triangle read by rows
 *
 *  Formula (recurrence for the triangle):
 *      T(n,n) = 1,
 *      T(n,k) = (k+1) * T(n,k+1) - Σ_{s=k+2..n} T(n,s)     for n>k .
 *
 *  The sequence a(n) is obtained by reading the triangle row‑by‑row
 *  (offset 0).  Thus a(n) = T(r,c) where r is the largest integer with
 *  r*(r+1)/2 ≤ n  and c = n - r*(r+1)/2  (0 ≤ c ≤ r).
 *
 *  The implementation generates only the single row that contains the
 *  requested entry, using the recurrence from right to left.
 *  Complexity:  O(r)  (r ≈ √(2n) for the position n).
 */

#include <stdlib.h>

/* Find the row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t
row_of_n(idx_t n)
{
    idx_t r = 0;
    /* use seq_t to avoid intermediate overflow when computing the
       triangle number (r+1)*(r+2)/2. */
    while ( ((seq_t)(r + 1) * (r + 2)) / 2 <= n )
        ++r;
    return r;
}

seq_t
a(idx_t n)
{
    idx_t r = row_of_n(n);                       /* row that contains a(n) */
    idx_t pref = r * (r + 1) / 2;                /* starting index of that row */
    idx_t c = n - pref;                          /* column within the row */

    /* Allocate space for the row; r is small (≈√(2n)), so the allocation is
       cheap and freed immediately after use. */
    seq_t *row = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!row)
        return (seq_t)0;                         /* fallback on allocation failure */

    row[r] = (seq_t)1;                           /* T(r,r) = 1 */

    seq_t sum = 0;                               /* S_{k+1} in the recurrence */
    for (idx_t k = r; k > 0; --k) {
        row[k - 1] = (seq_t)k * row[k] - sum;    /* T(r,k-1) = k * T(r,k) - S_{k} */
        sum += row[k];                           /* update S_{k} for next step */
    }

    seq_t result = row[c];
    free(row);
    return result;
}
