/*
 * A049769  Triangular array T read by rows: T(n,k) = (k^3 mod n) + (n^3 mod
 *          k).
 * https://oeis.org/A049769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r and k are chosen so that n = 1 + 2 + … +
 *            (r‑1) + k (row‑major order). T(r,k) = (k³ mod r) + (r³ mod k)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n) {
    /* n is 1‑based; for n==0 return 0 (undefined by OEIS). */
    if (n == 0)
        return 0;

    /* Find row r such that r(r+1)/2 >= n. */
    /* Compute floor sqrt of (1 + 8n). */
    idx_t tmp = (idx_t)(1 + 8 * n);
    idx_t s = int_sqrt(tmp);                /* floor sqrt */
    idx_t r = (s - 1) / 2;                  /* initial guess (floor) */
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Column k in this row. */
    idx_t prev = ((r - 1) * r) / 2;
    idx_t k = n - prev;

    /* Compute (k³ mod r) + (r³ mod k). */
    seq_t k_mod_r = ((seq_t)k * k) % r;
    seq_t k_cub_mod_r = (k_mod_r * k) % r;

    seq_t r_mod_k = ((seq_t)r * r) % k;
    seq_t r_cub_mod_k = (r_mod_k * r) % k;

    return k_cub_mod_r + r_cub_mod_k;
}
