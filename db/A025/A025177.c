/*
 * A025177  Triangular array, read by rows: first differences in n,n direction
 *          of trinomial array A027907.
 * https://oeis.org/A025177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = D(r,s) where r = floor(sqrt(n)) and s = n - r*r D(r,s) is
 *            the first difference in the n,n direction of the trinomial array
 *            (see OEIS A027907). D(r,s) = A(r,s) - A(r-1,s-1) where A(n,k) =
 *            sum_{i=0}^{⌊k/2⌋} binomial(n,i) * binomial(n-i, k-2i) with
 *            A(n,k)=0 for k<0 and using the recurrence T(...
 * Range:     u8 n <= 67, u16 n <= 178, u32 n <= 548, u64 n <= 1972,
 *            u128 n <= 2499, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t int_sqrt(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t A(idx_t n, idx_t k) {
    seq_t sum = 0;
    if (k < 0 || k > 2 * n) return 0;
    for (idx_t i = 0; 2 * i <= k; ++i) {
        idx_t kk = k - 2 * i;
        if (kk > n - i) continue;
        seq_t term = binomial(n, i) * binomial(n - i, kk);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n) {
    idx_t r = int_sqrt(n);      /* row */
    idx_t s = n - r * r;        /* offset in row */

    if (r == 0)                /* only element a(0) */
        return 1;

    seq_t Ars = A(r, s);
    seq_t Ars_prev;
    if (s == 0)
        Ars_prev = 0;
    else
        Ars_prev = A(r - 1, s - 1);

    return Ars - Ars_prev;
}
