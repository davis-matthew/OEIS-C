/*
 * A202674  Symmetric matrix based on (1,3,5,7,9,...), by antidiagonals.
 * https://oeis.org/A202674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) constructs a sequence from the symmetric matrix * M whose
 *            entries are M[i][j] = min(i,j) * (4*min(i,j)^2 * -
 *            6*(i+j)*min(i,j) + 12*i*j - 1) / 3. * The sequence lists M[i][j]
 *            by increasing i+j (anti‑diagonals), * with i ascending within
 *            each diagonal. * a(1) = M[1][1] = 1, a(2) = M[1][2] = 3,...
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t compute_M(idx_t i, idx_t j)
{
    idx_t k = (i < j) ? i : j;
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t kk = (seq_t)k;

    /* numerator = k * (4k^2 - 6(i+j)k + 12ij - 1) */
    seq_t numer = kk * (
        (seq_t)4 * kk * kk
        - (seq_t)6 * (ii + jj) * kk
        + (seq_t)12 * ii * jj
        - (seq_t)1);

    return numer / (seq_t)3;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 0, but return safe value */

    /* Find the smallest s with cumulative count >= n,
     * where count(s) = s*(s-1)/2 is the number of entries
     * up to and including the diagonal of sum s. */
    idx_t low = 2, high = 2;
    unsigned __int128 lim;

    while (1) {
        lim = (unsigned __int128)high * (high - 1) / 2;
        if (lim >= n) break;
        high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        lim = (unsigned __int128)mid * (mid - 1) / 2;
        if (lim >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t s = low;
    /* Count of items before diagonal s */
    lim = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t prev_count = (idx_t)lim;

    idx_t offset = n - prev_count - 1;   /* 0‑based position in diagonal s */
    idx_t i = offset + 1;
    idx_t j = s - i;

    return compute_M(i, j);
}
