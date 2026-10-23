/*
 * A143498  Triangle of unsigned 3-Lah numbers.
 * https://oeis.org/A143498
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of unsigned 3‑Lah numbers.
 * Computes a(n) – the flattened triangle: a(3)=T(3,3), a(4)=T(4,3), a(5)=T(4,4), …
 * Formula used:  T(r,k) = (r-3)!/(k-3)! * binomial(r+2, k+2)  for r,k ≥ 3.
 * a(n) is obtained by locating the row r and column k that correspond to
 * position n in the row‑major flattening, then evaluating T(r,k) as above.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    /* position m = n − 3 within the flattened sequence, starting at 0 */
    idx_t m = n - 3;

    /* Find s such that (s−1)*s/2 ≤ m < s*(s+1)/2 .
       Then r = s + 2  (row number) and k = (m − (s−1)*s/2) + 3 (column). */
    idx_t s = 1;
    while ( ( (unsigned __int128)s * (s + 1) ) / 2 <= m )
        ++s;

    idx_t r = s + 2;  /* row  ≥ 3 */
    idx_t offset_prev = ( (unsigned __int128)(s - 1) * s ) / 2;
    idx_t idx_in_row = m - offset_prev;            /* 0‑based index inside the row */
    idx_t k = idx_in_row + 3;                       /* column number ≥ 3 */

    /* Compute the triangle entry */
    seq_t value = binomial(r + 2, k + 2);           /* binomial(r+2,k+2) */

    /* Multiply by (k-2)(k-1)...(r-3) = (r-3)!/(k-3)! */
    if (k - 2 <= r - 3) {
        for (idx_t i = k - 2; i <= r - 3; ++i)
            value *= (seq_t)i;
    }

    return value;
}
