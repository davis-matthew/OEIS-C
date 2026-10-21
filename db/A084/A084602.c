/*
 * A084602  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1 + x + 3x^2)^n.
 * https://oeis.org/A084602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^(n - ⌊√n⌋²) in (1 + x + 3x²)^⌊√n⌋. The
 *            coefficient is given by: sum_{i=0}^{⌊m/2⌋} C(r,i) * C(r-i, m-2i)
 *            * 3^i where r = ⌊√n⌋ and m = n - r²
 * Range:     u8 n <= 29, u16 n <= 75, u32 n <= 272, u64 n <= 934,
 *            u128 n <= 2600, big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"

static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;  // floor(sqrt(n))
}

seq_t a(idx_t n)
{
    /* Determine row r and position m within that row. */
    idx_t r = isqrt(n);
    idx_t m = n - r * r;              /* 0 ≤ m ≤ 2r */

    seq_t sum = 0;
    for (idx_t i = 0; i <= m / 2; ++i) {
        idx_t j = m - 2 * i;
        /* The multinomial term is non‑zero only if j ≤ r-i. */
        if (j > r - i)
            continue;

        seq_t term = binomial(r, i);
        term = term * binomial(r - i, j);
        term = term * pow3(i);
        sum += term;
    }
    return sum;
}
