/*
 * A214859  Triangle read by rows, T(n,k) = n^2 - k*(k+1)/2 if k*(k+1)/2 <=
 *          n^2.
 * https://oeis.org/A214859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   Triangle read by rows, T(n,k) = n^2 - k*(k+1)/2 if k*(k+1)/2 ≤
 *            n^2, read by rows
 * Range:     u8 n <= 171, u16 n <= 9679, u32 n <= 9679, u64 n <= 9679,
 *            u128 n <= 9679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer
   returns ⌊√n⌋.  */
static inline uint32_t isqrt_u64(uint64_t n)
{
    uint64_t x  = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of 4 ≤ 2^64 */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return (uint32_t)x;
}

/* Maximum k for a given row n (0‑based) */
static inline uint32_t max_k_for_row(idx_t n)
{
    uint64_t n2   = (uint64_t)n * (uint64_t)n;
    uint64_t t    = 1 + 8ULL * n2;               /* 1 + 8n^2  */
    uint32_t sqt  = isqrt_u64(t);
    return (sqt - 1) / 2;
}

/* Sequence A214859 */
seq_t a(idx_t n)
{
    idx_t cum = 0;                           /* cumulative terms so far */

    for (idx_t row = 0; ; ++row) {
        uint32_t kmax = max_k_for_row(row);
        idx_t    len  = (idx_t)kmax + 1;     /* number of terms in this row */

        if (cum + len > n) {                /* desired term is in this row */
            idx_t k = n - cum;

            seq_t m_sq = (seq_t)row * (seq_t)row;
            seq_t subtract = ((seq_t)k * ((seq_t)k + 1)) / 2;
            return m_sq - subtract;
        }

        cum += len;
    }
}
