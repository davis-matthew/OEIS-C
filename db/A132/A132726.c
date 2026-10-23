/*
 * A132726  Triangle read by rows: T(n,k) = length of period in decimal
 *          representation of k/n, 1<=k<=n.
 * https://oeis.org/A132726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = length of the period of the decimal expansion of k / n
 *            where n is the row number and k = n - (n-1)(n-2)/2. The triangle
 *            is read row‑by‑row, so n maps to row r and column k by r =
 *            ceil((sqrt(8n+1)-1)/2), k = n - r(r-1)/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

/*-------------------------------------------------------------*/
/* Euclidean algorithm for idx_t (uint64_t).  This is safer
   than relying on the utilities' gcd functions.          */
static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*-------------------------------------------------------------*/
/* Binary search for the row r such that r(r+1)/2 >= n.         */
static inline idx_t row_of(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (t < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/*-------------------------------------------------------------*/
/* Length of the repeating part of the decimal expansion
   of k/n.  The fraction is reduced, factors of 2 and 5 are
   removed from the denominator, then the multiplicative order
   of 10 modulo the remaining denominator is found.          */
static inline seq_t period_length(idx_t k, idx_t n)
{
    /* Reduce the fraction k/n. */
    idx_t g = gcd_idx_t(k, n);
    idx_t denom = n / g;

    /* Remove factors of 2 and 5. */
    while ((denom & 1ULL) == 0)
        denom >>= 1;
    while (denom % 5ULL == 0)
        denom /= 5ULL;

    /* Terminating decimal. */
    if (denom == 1)
        return 0;

    /* Multiplicative order of 10 modulo denom. */
    idx_t len = 1;
    idx_t rem = 10 % denom;
    while (rem != 1) {
        rem = (rem * 10) % denom;
        ++len;
    }
    return (seq_t)len;
}

/*-------------------------------------------------------------*/
/* Public sequence function.                                     */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset safety – undefined behaviour otherwise */
        return 0;

    idx_t r = row_of(n);                       /* row */
    idx_t prev_tri = (r * (r - 1)) / 2;        /* number of entries before this row */
    idx_t k = n - prev_tri;                    /* column (1‑based) */

    return period_length(k, r);
}
