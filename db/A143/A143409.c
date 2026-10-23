/*
 * A143409  Square array read by antidiagonals: form the Euler-Seidel matrix
 *          for the sequence {k!} and then divide column k by k!.
 * https://oeis.org/A143409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A143409):
   a(n) is obtained by reading the Euler-Seidel matrix T(n,k) (for the
   sequence k!) by antidiagonals, where
     T(n,k) = sum_{j=0..n} binom(n,j) * (k+j)! / k!
   The index n is converted to coordinates (row, col) with
     row + col = d,
   d being the smallest integer with (d+1)(d+2)/2 > n.
   Then a(n) = T(row, col).  */

#include <stdint.h>

/* Helper: triangular number t(d) = (d+1)(d+2)/2  (d >= 0) */
static seq_t triangular_idx(idx_t d)
{
    return (seq_t)(((__int128)(d + 1) * (d + 2)) / 2);
}

/* Compute T(n,k) using the multiplicative form:
   T(n,k) = sum_{j=0..n} binom(n,j) * prod_{t=1..j}(k+t)  */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    seq_t C   = 1;   /* binom(n,0) */
    seq_t P   = 1;   /* product_{t=1..j}(k+t), initially 1 for j=0 */

    for (idx_t j = 0; j <= n; ++j) {
        sum += C * P;
        if (j == n) break;  /* avoid overflow in updates */

        /* Update binomial coefficient: C = C * (n-j) / (j+1) */
        seq_t num = C * (seq_t)(n - j);
        C = num / (seq_t)(j + 1);

        /* Update product for the next j: multiply by (k + j + 1) */
        P *= (seq_t)(k + j + 1);
    }
    return sum;
}

/* Main sequence function: a(n)  */
seq_t a(idx_t n)
{
    /* Find diagonal d such that t(d-1) <= n < t(d) */
    idx_t low  = 0;
    idx_t high = 1;
    seq_t high_cnt = triangular_idx(high);          /* t(high) */
    seq_t n_seq   = (seq_t)n;

    /* Exponentially increase high until t(high) > n */
    while (high_cnt <= n_seq) {
        high <<= 1;
        high_cnt = triangular_idx(high);
    }

    /* Binary search for minimal d with t(d) > n */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t mid_cnt = triangular_idx(mid);
        if (mid_cnt <= n_seq)
            low = mid;
        else
            high = mid;
    }
    idx_t d = high;

    /* Compute offset within diagonal */
    seq_t prev_cnt = (seq_t)(((__int128)d * (d + 1)) / 2);  /* t(d-1) */
    idx_t offset = n - (idx_t)prev_cnt;

    /* Coordinates for T(row, col) */
    idx_t row = d - offset;
    idx_t col = offset;

    /* Return the table entry */
    return compute_T(row, col);
}
