/*
 * A204030  Symmetric matrix based on f(i,j) = gcd(i+1, j+1), by
 *          antidiagonals.
 * https://oeis.org/A204030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(i+1, j+1) where (i, j) is the nth entry of the
 *            infinite matrix M(i, j) = gcd(i+1, j+1) read by antidiagonals (i
 *            + j increasing, i ascending)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* Find the diagonal index k such that k*(k-1)/2 >= n. */
    idx_t low = 1;
    idx_t high = n + 1;                  /* high is always an upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;

    /* Compute the number of terms before this diagonal. */
    __uint128_t prev = (__uint128_t)(k - 1) * (k - 2) / 2;
    idx_t prev_T = (idx_t)prev;

    /* Zero‑based offset within the diagonal. */
    idx_t offset = n - prev_T - 1;
    idx_t i = offset + 1;               /* 1‑based row index within the diagonal */
    idx_t j = k - i;                    /* 1‑based column index within the diagonal */

    /* The entries of the matrix are gcd(i+1, j+1). */
    seq_t g1 = (seq_t)(i + 1);
    seq_t g2 = (seq_t)(j + 1);
    return gcd(g1, g2);
}
