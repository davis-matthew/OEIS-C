/*
 * A159624  a(n)=A159619(2n)-A159615(2n).
 * https://oeis.org/A159624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A159619(2n) - A159615(2n) Equivalent piecewise
 *            description: For n ≥ 2, let k = ⌊log₂ n⌋. • If 2ᵏ ≤ n ≤ 2ᵏ +
 *            2^(k‑1) – 1 → a(n) = 2ᵏ + 2 • If n = 2ᵏ + 2^(k‑1) + j with 0 ≤ j
 *            ≤ 2^(k‑1) – 1 → a(n) = 2ᵏ + 2j + 3 The special case n = 1 gives
 *            a(1) = 3
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of log₂ for a 64‑bit unsigned integer. */
static inline idx_t _floor_log2_idx(idx_t n)
{
    idx_t k = 0;
    while (n > 1) {
        n >>= 1;
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return 3;
    }

    idx_t k = _floor_log2_idx(n);      /* k = floor(log2(n))   */
    seq_t pow2k = (seq_t)((uint64_t)1 << k);   /* 2ᵏ                 */
    seq_t half_pow2k = pow2k >> 1;          /* 2^(k‑1)              */

    /* Convert the boundary to idx_t for the comparison. */
    idx_t boundary = (idx_t)pow2k + (idx_t)half_pow2k;

    if (n < boundary) {
        /* First sub‑interval: a(n) = 2ᵏ + 2 */
        return pow2k + 2;
    } else {
        /* Second sub‑interval: a(n) = 2ᵏ + 2j + 3 */
        idx_t j = n - boundary; /* 0 ≤ j ≤ 2^(k‑1) – 1 */
        seq_t a_value = pow2k + ((seq_t)j << 1) + 3;
        return a_value;
    }
}
