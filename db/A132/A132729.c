/*
 * A132729  Triangle T(n, k) = 2*binomial(n, k) - 3 with T(n, 0) = T(n, n) =
 *          1, read by rows.
 * https://oeis.org/A132729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 610, u64 n <= 2307,
 *            u128 n <= 5150, big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A132729: A(n) is the n‑th element of the triangle
   T(n,k) = 2*binomial(n,k)-3 (interior),  T(n,0)=T(n,n)=1,
   read row‑wise starting with row 0.  For even seq_t the
   result fits in the type.  For arbitrary‑precision seq_t
   an exact value is returned. */

#include <stdint.h>
#include "../utils/binom_n_k.h"

/* Find the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) low = mid;
        else high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate row r and column k within that row */
    idx_t r = row_of_index(n);
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;

    /* Boundary terms are always 1 */
    if (k == 0 || k == r) return (seq_t)1;

    /* Use symmetry to minimise work */
    if (k > r / 2) k = r - k;

    /* For small seq_t (≤ 64 bits) compute binomial with 128‑bit
       arithmetic to avoid intermediate overflow */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 bin = 1;
        for (idx_t i = 1; i <= k; ++i) {
            bin = bin * (r - k + i) / i;
        }
        unsigned __int128 res = bin * 2 - 3;
        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t, use the utility binom_n_k
       and rely on the type’s arithmetic operators */
    seq_t bin = binom_n_k(r, k);
    seq_t res = bin * 2;
    res = res - (seq_t)3;
    return res;
}
