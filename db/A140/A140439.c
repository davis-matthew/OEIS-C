/*
 * A140439  Array, by antidiagonals, arises in counting <= k facets in
 *          d-dimensional n-point sets.
 * https://oeis.org/A140439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (d + 1) * C(d + k, k) , where d ≥ 2, k ≥ 0 are obtained
 *            from n by 1. Find the minimal S ≥ 2 with (S‑1)*S/2 ≥ n (sum of
 *            terms up to diagonal S). 2. prev = (S‑2)*(S‑1)/2 – number of
 *            terms before diagonal S. 3. offset = n – prev – 1 (0‑based index
 *            inside diagonal S). 4. d = S – offset, k = of...
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n=0, return 0 */

    /* find S such that (S-1)*S/2 >= n */
    idx_t S = 2;
    while ((((__int128)(S - 1) * S) / 2) < n) {
        ++S;
    }

    /* number of terms before this diagonal */
    idx_t prev = ((S - 2) * (S - 1)) / 2;
    idx_t offset = n - prev - 1;         /* 0‑based position inside diagonal */

    idx_t d = S - offset;                /* d ≥ 2 */
    idx_t k = offset;                    /* k ≥ 0 */

    seq_t bin = binomial(d + k, k);
    seq_t result = ((seq_t)(d + 1)) * bin;
    return result;
}
