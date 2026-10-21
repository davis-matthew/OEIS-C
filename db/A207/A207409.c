/*
 * A207409  Triangular array: T(n,k) = prime(n) mod prime(k), 1 <= k < n.
 * https://oeis.org/A207409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(r) mod prime(k) where r and k are derived from the
 *            triangular array T(r,k) = prime(r) mod prime(k) (1 ≤ k < r) by
 *            enumerating the array row‑by‑row starting at n = 2. The integer
 *            m = n-2 is zero‑based; find the smallest r such that r(r-1)/2 >
 *            m, let k = m - (r-2)(r-1)/2 + 1, and retur...
 * Range:     u8 n <= 1870, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)                         /* no defined terms before offset */
        return 0;

    idx_t m = n - 2;                    /* zero‑based array index   */
    idx_t r = 2;                        /* start with first row (r=2) */
    while ((r - 1) * r / 2 <= m)        /* find row containing m   */
        r++;

    /* previous cumulative entries up to row r-1 */
    idx_t prev = (r - 2) * (r - 1) / 2;
    idx_t k = m - prev + 1;             /* column index (1‑based)   */

    /* compute the required entry */
    seq_t prime_r = (seq_t)get_prime(r);
    seq_t prime_k = (seq_t)get_prime(k);

    return prime_r % prime_k;
}
