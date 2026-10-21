/*
 * A089654  Table T(n,k), read by rows, related to a conjecture of P. Erdos
 *          (see A039669).
 * https://oeis.org/A089654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = T(m,k) = 2*m + 1 - 2^k, where m is the smallest positive
 *            integer with Σ_{i=1}^{m-1} (⌊log₂ i⌋ + 1) < n ≤ Σ_{i=1}^{m}
 *            (⌊log₂ i⌋ + 1) and k = n - Σ_{i=1}^{m-1} (⌊log₂ i⌋ + 1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; below may be arbitrary */

    idx_t cumulative = 0;
    idx_t row        = 0;
    idx_t len        = 0;

    while (cumulative < n) {
        ++row;
        len = floor_log2_idx(row) + 1;
        cumulative += len;
    }

    idx_t offset = n - (cumulative - len);          /* 1‑based position within row */

    seq_t term = (seq_t)row * 2 + 1;                 /* 2*m + 1 */
    term -= (seq_t)((uint64_t)1 << offset);          /* subtract 2^k */

    return term;
}
