/*
 * A143271  Triangle read by rows: A130209 * A000012 * A127648.
 * https://oeis.org/A143271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = d(r) * k * where r is the unique row index such that *
 *            (r‑1)·r/2 < n ≤ r·(r+1)/2 * and k = n – (r‑1)·r/2. * d(r) is
 *            τ(r), the number of divisors of r
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t divisor_count(idx_t num)
{
    seq_t cnt = 1;
    idx_t m   = num;
    for (idx_t p = 2; p <= m / p; ++p) {
        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp) cnt *= (exp + 1);
    }
    if (m > 1) cnt *= 2;            /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    /* Find row r by binary search on triangular numbers:  r(r+1)/2 ≥ n */
    idx_t low  = 1;
    idx_t high = 1;
    while (((seq_t)high * (high + 1)) / 2 < (seq_t)n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((seq_t)mid * (mid + 1)) / 2 >= (seq_t)n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t r = low;

    /* Position k within the row */
    seq_t tri_prev = ((seq_t)(r - 1) * r) / 2;
    idx_t k        = (idx_t)((seq_t)n - tri_prev);
    seq_t k_seq    = (seq_t)k;

    /* Value = τ(r) * k */
    return divisor_count(r) * k_seq;
}
