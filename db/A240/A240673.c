/*
 * A240673  Triangle read by rows: T(n, k) = v(n, k)*((1/v(n, k)) mod
 *          prime(k)), where v(n, k) = (Product_{j=1..n} prime(j))/prime(k), n
 *          >= 1, 1 <= k <= n.
 * https://oeis.org/A240673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row containing the n-th element
 *            (r(r+1)/2 ≥ n) and k = n - r(r-1)/2 is the column in that row.
 *            T(r,k) = v(r,k) * inv(v(r,k) mod prime(k), prime(k)), with
 *            v(r,k) = ∏_{j=1..r, j≠k} prime(j)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 50, u64 n <= 121,
 *            u128 n <= 351, big any n
 * Helpers:   get_prime, mod_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1. For n < 1 the behavior is undefined. */
    if (n < 1) return 0;

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column k in the row. */
    idx_t k = n - (r - 1) * r / 2;   /* 1 ≤ k ≤ r */

    /* Compute v(r,k) = ∏_{j=1..r, j≠k} prime(j). */
    seq_t v = 1;
    for (idx_t j = 1; j <= r; ++j) {
        if (j == k) continue;
        uint32_t pj = get_prime(j);
        v = seq_mul(v, (seq_t)pj);
    }

    /* Modular inverse of v modulo prime(k). */
    uint32_t pk = get_prime(k);
    seq_t v_mod = v % pk;                     /* v mod pk  */
    seq_t inv   = mod_pow(v_mod, (uint64_t)pk - 2, (seq_t)pk);

    /* Return T(r,k) = v * inv. */
    return seq_mul(v, inv);
}
