/*
 * A185249  Triangle read by rows: Table III.5 of Myriam de Sainte-Catherine's
 *          1983 thesis.
 * https://oeis.org/A185249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2, and T(n,k) = ∏_{1 ≤ i ≤ j ≤ k} (n-k+i+j)/(i+j) if n-k
 *            is even, otherwise 0
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 128, u64 n <= 241,
 *            u128 n <= 451, big any n
 * Helpers:   _seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_seq_gcd.h"

static seq_t compute_T(idx_t n, idx_t k)
{
    /* Result is zero if the parity condition fails. */
    if (((n - k) & 1ULL) != 0ULL)
        return (seq_t)0;

    if (k == 0)          /* product over empty set */
        return (seq_t)1;

    seq_t num = 1;
    seq_t den = 1;
    for (idx_t i = 1; i <= k; ++i) {
        for (idx_t j = i; j <= k; ++j) {
            seq_t a = (seq_t)(n - k + i + j);
            seq_t b = (seq_t)(i + j);
            num *= a;
            den *= b;
            seq_t g = _seq_gcd(num, den);
            if (g > 1) {
                num /= g;
                den /= g;
            }
        }
    }
    /* After all reductions, den should be 1. */
    return num;
}

seq_t a(idx_t n)
{
    /* Find the row r containing index n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t prev = r * (r + 1) / 2;   /* cumulative terms before row r */
    idx_t k = n - prev;             /* column within that row */

    return compute_T(r, k);
}
