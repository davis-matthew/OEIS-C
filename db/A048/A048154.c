/*
 * A048154  Triangular array T read by rows: T(n,k)=k^3 mod n, for
 *          k=1,2,...,n, n=1,2,...
 * https://oeis.org/A048154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k^3 mod m) where m = ⌈(√(1+8·n)−1)/2⌉ (row index) k = n
 *            − (m−1)m/2 (column index in that row)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 107 terms through n = 4860 (2026-11-01)
 * Helpers:   isqrt_i128_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_i128_to_idx.h"

seq_t a(idx_t n)
{
    /* Compute m = ceil((sqrt(1+8n)-1)/2) */
    __int128 t = (__int128)1 + (__int128)8 * n;      /* 1+8n fits in 128 bits */
    idx_t sqrt_t = isqrt_i128_to_idx(t);
    idx_t m = (sqrt_t - 1) / 2;                      /* floor part */
    /* Adjust if necessary */
    if ((((__int128)m * (m + 1)) >> 1) < n) {
        ++m;
    }

    /* Column index k */
    __int128 prev = (((__int128)(m - 1)) * m) >> 1;   /* (m-1)m/2 */
    idx_t k = n - (idx_t)prev;

    /* Compute k^3 mod m in seq_t */
    seq_t m_seq = (seq_t)m;
    seq_t k_seq = (seq_t)k;
    seq_t tmp    = (k_seq * k_seq) % m_seq;          /* k^2 mod m */
    seq_t result = (tmp * k_seq) % m_seq;            /* k^3 mod m */

    return result;
}
