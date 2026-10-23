/*
 * A135233  Triangle A007318 * A193554, read by rows.
 * https://oeis.org/A135233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   triangle A007318 * A193554, read by rows – a(n) is the n‑th
 *            element of the flattened triangle: T(n,k) = sum_{j=0..n-k}
 *            (-1)^(n-k+j) * binomial(n,j) * 2^j, with T(n,0) = (3^n + 1)/2,
 *            T(n,n) = 1
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 230, u64 n <= 902,
 *            u128 n <= 3402, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required utilities */
#include <stdint.h>
#include <math.h>
#include "../utils/pow3.h"

/* Forward */
static seq_t T_value(idx_t n, idx_t k);

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find the row `r` that contains index `n` */
    idx_t r = (idx_t)((sqrt((double)8.0 * n + 1.0) - 1.0) / 2.0);

    /* Adjust `r` in case of floating‑point rounding errors */
    while ((r * (r + 1)) / 2 > n) --r;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    /* Offset of the first element of row `r` */
    idx_t offset = (r * (r + 1)) / 2;

    /* Column index inside the row */
    idx_t k = n - offset;

    /* Return the value of the triangle entry */
    return T_value(r, k);
}

/* Compute T(r,k) using the recurrence that never produces negative intermediates. */
static seq_t T_value(idx_t n, idx_t k)
{
    /* Edge cases */
    if (k == n) return 1;                     /* T(n,n) = 1 */
    if (k == 0) {                             /* T(n,0) = (3^n + 1)/2 */
        seq_t t = pow3(n) + 1;
        return t / 2;
    }

    /* Compute from the rightmost element backward to k */
    seq_t T_next = 1;          /* T(n,n) */
    seq_t binom  = 1;          /* C(n,0) */
    seq_t pow2   = 1;          /* 2^0 */

    for (idx_t m = 1; m <= n - k; ++m) {
        /* Update binomial coefficient: C(n,m) = C(n,m-1) * (n-(m-1)) / m */
        binom = binom * (seq_t)(n - (m - 1));
        binom = binom / m;

        /* Update power of two */
        pow2 = pow2 * (seq_t)2;

        /* Current term: C(n,m) * 2^m */
        seq_t term = binom * pow2;

        /* T(n, n - m) = term - T_next  (term ≥ T_next for 1 ≤ k ≤ n-1) */
        T_next = term - T_next;
    }

    return T_next;   /* T(n,k) */
}
