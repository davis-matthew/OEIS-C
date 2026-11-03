/*
 * A226167  Array read by antidiagonals: a(i,j) is the number of ways of
 *          labeling a tableau of shape (i,1^j) with the integers 1, 2, ...
 *          i+j-2 (each label being used once) such that the first row is
 *          decreasing, and the first column has m-1 labels.
 * https://oeis.org/A226167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = array read by antidiagonals of * a(i,j) = ((i+j-2)! / i!)
 *            * ((2*i + j - 1) * j / 2) * * Implementation avoids factorial
 *            overflow by computing the * ratio (i+j-2)!/i! as a product of
 *            consecutive integers. * * The function is thread‑safe and works
 *            for any build * of seq_t (uint8_t ... uint64...
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 78, u64 n <= 190,
 *            u128 n <= 561, big any n
 * Validated: standard, pass, 71 terms through n = 9462 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the smallest integer s such that s*(s-1)/2 >= n.
 * Uses 128‑bit arithmetic to avoid intermediate overflow
 * even for large n. */
static inline idx_t find_diag_sum(idx_t n)
{
    idx_t low = 1, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, return 0 for safety */
        return 0;

    /* Find diagonal s = i + j with the nth term */
    idx_t s = find_diag_sum(n);

    /* Number of terms before this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;

    /* Position inside the diagonal (1‑based) */
    idx_t pos = n - prev;
    idx_t i   = pos;
    idx_t j   = s - pos;

    /* Handle j == 1 separately (a(i,1) == 1) */
    if (j == 1)
        return 1;

    /* Compute ((i+j-2)! / i!) as product from i+1 to i+j-2 */
    seq_t prod = 1;
    if (j >= 3) { /* j-2 >= 1 */
        idx_t start = i + 1;
        idx_t end   = i + j - 2;
        for (idx_t k = start; k <= end; ++k)
            prod *= (seq_t)k;
    }

    /* Compute ((2*i + j - 1) * j / 2) */
    seq_t factor_even = ((seq_t)(2 * i + j - 1) * (seq_t)j);
    seq_t result = prod * factor_even / 2;

    return result;
}
