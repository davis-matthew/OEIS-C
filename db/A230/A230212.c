/*
 * A230212  Trapezoid of dot products of row 9 (signs alternating) with
 *          sequential 10-tuples read by rows in Pascal's triangle A007318:
 *          T(n,k) is the linear combination of the 10-tuples (C(9,0),
 *          -C(9,1), ..., C(9,8), -C(9,9)) and (C(n-1,k-9), C(n-1,k-8), ...,
 *          C(n-1,k)), n >= 1, 0 <= k <= n+8.
 * https://oeis.org/A230212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = T(r,k) where r and k are obtained from the 1‑based *
 *            index n as follows: * r is the smallest integer such that (r^2 +
 *            19r)/2 ≥ n, * pref(r-1) = ((r-1)^2 + 19(r-1))/2, * k = n -
 *            pref(r-1) - 1 (0 ≤ k ≤ r+8). * Then T(r,k) = Σ_{i=0..9}
 *            (-1)^(i+9)·C(9,i)·C(r‑1, k‑i). * The signs are already in...
 * Range:     i8 n <= 171, i16 n <= 464, i32 n <= 829, i64 n <= 1725,
 *            i128 n <= 1725, big any n
 * Helpers:   binomial, row_for
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/row_for.h"

static const seq_t row9[10] = {
    - (seq_t)1,   /* (-1)^9 * C(9,0) */
    (seq_t)9,    /* (-1)^10 * C(9,1) */
    - (seq_t)36,  /* (-1)^11 * C(9,2) */
     (seq_t)84,  /* (-1)^12 * C(9,3) */
    - (seq_t)126,/* (-1)^13 * C(9,4) */
     (seq_t)126,/* (-1)^14 * C(9,5) */
    - (seq_t)84, /* (-1)^15 * C(9,6) */
     (seq_t)36, /* (-1)^16 * C(9,7) */
    - (seq_t)9,  /* (-1)^17 * C(9,8) */
     (seq_t)1   /* (-1)^18 * C(9,9) */
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, return 0 just in case */

    /* Determine row r and the column k within that row. */
    idx_t r   = row_for(n);                /* r >= 1 */
    __int128 prev = (__int128)(r-1) * (r-1) + 19 * (r-1);
    prev >>= 1;                            /* (r-1)^2/2 + 9*(r-1) */
    idx_t k = n - (idx_t)prev - 1;          /* 0 <= k <= r+8 */

    idx_t n_minus_1 = r - 1;
    seq_t result = 0;

    for (idx_t i = 0; i < 10; ++i) {
        if (k < i) continue;                      /* k-i < 0 */
        idx_t j = k - i;                          /* k-i */
        if (j > n_minus_1) continue;              /* out of range */
        seq_t bin = binomial(n_minus_1, j);        /* C(r-1, k-i) */
        result += row9[i] * bin;
    }
    return result;
}
