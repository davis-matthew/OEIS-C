/*
 * A026249  a(n) = j if n = [ j*sqrt(2) ], else a(n) = k if n = [ k*(2 +
 *          sqrt(2)) ].
 * https://oeis.org/A026249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A026249:
   a(n) = j  if  n = ⌊ j*√2 ⌋
          k  if  n = ⌊ k*(2+√2) ⌋
   The two Beatty sequences are complementary, so one of the equalities
   always holds. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS offset */
    seq_t target = (seq_t)n;          /* convert to seq_t for arithmetic */
    idx_t k;                          /* candidate index */

    /* ---------------------------------- */
    /* Search the √2 Beatty sequence      */
    /* ---------------------------------- */
    idx_t lo = 1, hi = n;             /* k cannot exceed n */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);

        /* m = ⌊√(2*mid*mid)⌋ = ⌊mid*√2⌋ */
        seq_t m_val = floor_sqrt((seq_t)mid * (seq_t)mid * 2ULL);

        if (m_val <= target) {
            lo = mid + 1;
            k  = mid;                 /* store the largest mid with m ≤ target */
        } else {
            hi = mid - 1;
        }
    }

    /* Verify whether target equals ⌊k*√2⌋ */
    seq_t m_check = floor_sqrt((seq_t)k * (seq_t)k * 2ULL);
    if (m_check == target) {
        return (seq_t)k;
    }

    /* ---------------------------------- */
    /* Search the (2+√2) Beatty sequence  */
    /* ---------------------------------- */
    lo = 1;
    hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);

        seq_t m_val = floor_sqrt((seq_t)mid * (seq_t)mid * 2ULL);
        seq_t beta  = 2 * (seq_t)mid + m_val;   /* ⌊mid*(2+√2)⌋ */

        if (beta <= target) {
            lo = mid + 1;
            k  = mid;                  /* largest mid with beta ≤ target */
        } else {
            hi = mid - 1;
        }
    }

    /* The sequence is complementary, so this must match */
    return (seq_t)k;
}
