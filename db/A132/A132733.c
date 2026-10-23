/*
 * A132733  Triangle T(n, k) = 4*binomial(n, k) - 5 with T(n, 0) = T(n, n) =
 *          1, read by rows.
 * https://oeis.org/A132733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is the first or last element of a row, otherwise
 *            a(n) = 4 * C(r, k) – 5, where r = floor((sqrt(8*n+1)-1)/2) and k
 *            = n – r*(r+1)/2. The triangle entry T(r,k)=4*C(r,k)-5 with
 *            T(r,0)=T(r,r)=1
 * Range:     u8 n <= 39, u16 n <= 159, u32 n <= 576, u64 n <= 2240,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No use of floating point, no external headers needed.  */
#include <stdint.h>
#include <stdbool.h>

static inline seq_t binom_idx(idx_t n, idx_t k)
{
    /* Compute C(n,k) using 128-bit intermediate to avoid overflow for
       values that fit into the target type.  */
    if (k > n - k)
        k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1).  */
    idx_t r = 0;
    while (1) {
        unsigned __int128 next = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (next > n)
            break;
        ++r;
    }

    /* Starting index of row r */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;          /* column within the row */

    if (k == 0 || k == r) {
        return (seq_t)1;
    } else {
        seq_t bin = binom_idx(r, k);
        return bin * 4 - 5;
    }
}
