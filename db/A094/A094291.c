/*
 * A094291  a(n) = maximal value of C(i, j) * C(n-j, n-i) for 0 <= j <= i <=
 *          n.
 * https://oeis.org/A094291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{0<=j<=i<=n} C(i,j) * C(n-j,n-i)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 22, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes A094291 for arbitrary‐precision seq_t or fixed‐size integer types */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* sequence starts at n=1 */

    /* Result holder */
    seq_t max_val = 0;

    /* We only need j up to floor(n/2);  for larger j C(n-j,j)=0 */
    idx_t jmax = n / 2;

    /* b_j = C(n-j,j)   (computed iteratively) */
    seq_t b = 1;          /* b_0 */
    seq_t b_sq = 1;       /* b^2 for current j */

    /* j = 0 case */
    seq_t t2 = b_sq * (seq_t)n * (seq_t)n;          /* n^2   */
    t2 = t2 / ((seq_t)n * (seq_t)1);                /* cancel n  →  n  */
    if (t2 > max_val) max_val = t2;
    if (b_sq > max_val) max_val = b_sq;

    /* Iterate over j = 1 .. floor(n/2) */
    for (idx_t j = 1; j <= jmax; ++j) {
        /* Recurrence:
           b_j = b_{j-1} * ((n-2j+2)*(n-2j+1)) / (j*(n-j+1))   */
        seq_t num = b * ((seq_t)(n - 2*j + 2) * (seq_t)(n - 2*j + 1));
        seq_t den = (seq_t)(j * (n - j + 1));
        b   = num / den;
        b_sq = b * b;

        if (b_sq > max_val) max_val = b_sq;

        /* second candidate (k = j+1) is valid only when n >= 2j+1 */
        if (n >= 2*j + 1) {
            seq_t diff = (seq_t)(n - 2*j);               /* n-2j  */
            seq_t num2 = b_sq * diff * diff;            /* b^2*(n-2j)^2 */
            seq_t den2 = (seq_t)(n - j) * (seq_t)(j + 1); /* (n-j)(j+1) */
            seq_t t2   = num2 / den2;
            if (t2 > max_val) max_val = t2;
        }
    }

    return max_val;
}
