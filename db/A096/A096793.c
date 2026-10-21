/*
 * A096793  Triangle read by rows: a(n,k) is the number of Dyck n-paths
 *          containing k odd-length ascents.
 * https://oeis.org/A096793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   the flattened triangle read by rows of * a(n,k) =
 *            binomial((n+k)/2,(n-k)/2)*binomial((3n-k)/2+1,(n+k)/2)/((3n-k)/2+1).
 *            * The function returns the nth element of that flattened list
 * Range:     u8 n <= 20, u16 n <= 66, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, mul_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_div.h"

/* Compute the triangle entry a(r,k) using the above formula. */
static inline seq_t triangle_entry(idx_t r, idx_t k)
{
    if (k > r)                 return (seq_t)0;
    if ((r & 1) != (k & 1))    return (seq_t)0;   /* parity mismatch */

    idx_t a1 = (r + k) / 2;
    idx_t b1 = (r - k) / 2;
    seq_t term1 = binomial(a1, b1);

    idx_t temp = (3 * r - k) / 2;
    idx_t a2   = temp + 1;     /* denominator and upper argument of second binomial */
    idx_t b2   = a1;
    seq_t term2 = binomial(a2, b2);

    return mul_div(term1, term2, a2);
}

/* Return the n‑th element of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Find the row r so that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - start;
    return triangle_entry(r, k);
}
