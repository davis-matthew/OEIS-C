/*
 * A108497  Triangle read by rows: T(n,k) = k^(n+1)-k mod n, showing 1<=k<=n.
 * https://oeis.org/A108497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k^(r+1)-k) mod r where (r,k) is the row and column * of
 *            the flatten index n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   int_sqrt_idx, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt_idx.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r;
    {
        idx_t s = int_sqrt_idx(8 * n + 1); /* floor(sqrt(8n+1)) */
        r = (s - 1) / 2;
        while (r * (r + 1) / 2 < n) ++r;                    /* adjust upward */
        while (r > 1 && (r - 1) * r / 2 >= n) --r;          /* adjust downward */
    }

    /* Position k within the row (1 ≤ k ≤ r) */
    idx_t prev = (r - 1) * r / 2;     /* number of entries before this row */
    idx_t k = n - prev;               /* column index */

    /* Compute (k^(r+1)-k) mod r */
    uint64_t pow_mod = mod_pow((uint64_t)k, (uint64_t)(r + 1), (uint64_t)r); /* k^(r+1) mod r */
    seq_t result = (seq_t)((pow_mod + r - (seq_t)k) % r);                    /* (pow_mod - k) mod r */

    return result;
}
