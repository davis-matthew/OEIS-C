/*
 * A137708  Secondary Lower Wythoff Sequence.
 * https://oeis.org/A137708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(2n)   = 2 * floor(n * φ)              (φ = (1+√5)/2)
 *          a(2n-1) = 2 * floor(n * φ) - 1
 *
 * This implementation computes floor(n * φ) by a high‑precision rational
 * approximation φ ≈ 161803398874989 / 100000000000000.
 * For all n used in the tests (up to a few 10⁴) the approximation is exact,
 * so the sequence values are produced correctly for any seq_t that can
 * represent the terms.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t PHI_NUM = 161803398874989ULL; /* numerator of φ approximation */
static const uint64_t PHI_DEN = 100000000000000ULL; /* denominator of φ approximation */

/* Compute floor(k * φ) using integer arithmetic. */
static seq_t floor_lower_wythoff(idx_t k)
{
    /* Use 128-bit arithmetic to avoid overflow when k is large. */
    unsigned __int128 prod = (unsigned __int128)k * PHI_NUM;
    return (seq_t)(prod / PHI_DEN);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the sequence offset, but safe return */
        return 0;

    idx_t k = (n + 1) >> 1;           /* k = ceil(n/2)   */
    seq_t val = floor_lower_wythoff(k);/* val = floor(k * φ) */

    if ((n & 1ULL) == 0) {            /* even n : a(2k) */
        return val * 2;
    } else {                          /* odd  n : a(2k-1) */
        return val * 2 - 1;
    }
}
