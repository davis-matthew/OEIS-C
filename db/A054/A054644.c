/*
 * A054644  Number of labeled pure 2-complexes on n nodes with 3 2-simplexes.
 * https://oeis.org/A054644
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(binomial(n,3),3) for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 27, u64 n <= 307, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* offset starts at n = 4 */

    /* compute C(n,3) = n*(n-1)*(n-2)/6   using 128‑bit intermediate */
    __int128 nn = n;
    __int128 bin_n3 = nn * (n - 1) * (n - 2) / 6;
    seq_t k = (seq_t)bin_n3;      /* k = C(n,3) */

    /* compute C(k,3) = k*(k-1)*(k-2)/6   also with 128‑bit intermediate */
    __int128 kk = bin_n3;
    __int128 bin_k3 = kk * (kk - 1) * (kk - 2) / 6;
    return (seq_t)bin_k3;
}
