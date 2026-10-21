/*
 * A060039  Triangle T(n,k) = 1 if k-th prime is a square mod the n-th prime,
 *          otherwise 0, for 1<=k<n.
 * https://oeis.org/A060039
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = 1 if the k‑th prime is a quadratic residue modulo the
 *            r‑th prime, where r and k are obtained from n by flattening the
 *            triangle T(r,k) (offset 2); otherwise 0
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   _floor_isqrt_u64, get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/get_prime.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* pathologically out of range */
    idx_t m = n - 1;                /* 1‑based index inside the flattened triangle */

    /* Find smallest r such that r*(r-1)/2 >= m */
    idx_t s = _floor_isqrt_u64(1 + 8 * m);
    idx_t r = (1 + s) / 2;
    if (r * (r - 1) / 2 < m) r++;   /* adjust if necessary */

    idx_t prev = (r - 2) * (r - 1) / 2;   /* entries before row r */
    idx_t k = m - prev;                   /* 1‑based position within row r */

    uint32_t pr = get_prime(r);         /* r‑th prime */
    uint32_t pk = get_prime(k);         /* k‑th prime */

    int jacobi = jacobi_symbol((uint64_t)pk, (uint64_t)pr); /* Legendre symbol (pk|pr) */
    return (seq_t)(jacobi == 1 ? 1 : 0);
}
