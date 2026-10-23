/*
 * A133704  A051731 * a diagonalized matrix of A133696.
 * https://oeis.org/A133704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 0 if (r+1) mod (k+1) ≠ 0 a(n) = 2*μ(k+1) – 1 otherwise
 *            where r = ⌊(√(8n+1) – 1)/2⌋ k = n – r(r+1)/2 (μ is the Möbius
 *            function)
 * Range:     i8 n <= 54, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the Möbius value of m (±1 or 0). */
static seq_t mobius(idx_t m)
{
    seq_t val = 1;                     /* μ(1) = 1 */
    idx_t i = 2;
    while (i <= m / i) {
        if (m % i == 0) {
            m /= i;
            if (m % i == 0)            /* squared prime factor → μ = 0 */
                return (seq_t)0;
            val = -val;                /* toggle sign for each distinct prime */
        }
        ++i;
    }
    if (m > 1)                          /* remaining prime factor */
        val = -val;
    return val;
}

/* Determine r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2
   using binary search with 128‑bit intermediates. */
static idx_t row_from_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
        if (high == 0)                /* overflow safeguard */
            break;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row r and column k. */
    idx_t r = row_from_index(n);
    idx_t k = n - r * (r + 1) / 2;

    idx_t r_plus = r + 1;
    idx_t k_plus = k + 1;

    if (r_plus % k_plus != 0)
        return (seq_t)0;

    seq_t mu = mobius(k_plus);
    return (seq_t)(2 * mu - 1);
}
