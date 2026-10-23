/*
 * A038314  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*12^j.
 * https://oeis.org/A038314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 10^(i-j) * 12^j, where i =
 *            floor((sqrt(1+8n)-1)/2) and j = n - i*(i+1)/2 enumerating the
 *            triangle row‑by‑row (i≥j)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 38, u64 n <= 126, u128 n <= 475,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row index i by binary search on the cumulative count
       of entries up to row i-1: i(i+1)/2 <= n < (i+1)(i+2)/2. */
    idx_t low = 0, high = n, mid, i = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (tri > n) {
            i = mid;
            if (mid == 0) break;  /* avoid underflow when high becomes -1 */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Compute j = n - i*(i+1)/2. */
    unsigned __int128 off128 = (unsigned __int128)i * (i + 1) / 2;
    idx_t offset = (idx_t)off128;
    idx_t j = n - offset;

    /* Compute binomial coefficient C(i,j). */
    seq_t bin = binomial(i, j);

    /* Compute 10^(i-j) and 12^j. */
    seq_t pow10 = seq_pow((seq_t)10, i - j);
    seq_t pow12 = seq_pow((seq_t)12, j);

    /* Return the product bin * pow10 * pow12. */
    return seq_mul(seq_mul(bin, pow10), pow12);
}
