/*
 * A117082  Octagonal numbers for which the sum of the digits is also an
 *          octagonal number.
 * https://oeis.org/A117082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   A117082 – octagonal numbers whose digit sum is also octagonal *
 *            a(n) = the n‑th octagonal number O_k = k(3k‑2) for which
 *            digit_sum(O_k) * is again an octagonal number
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 952 (2026-11-01)
 * Helpers:   digit_sum, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
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

/* Test whether x is an octagonal number:  x = k(3k‑2) for some k ≥ 0 */
static bool is_octagonal(seq_t x)
{
    if (x == 0)                /* 0 is defined to be octagonal */
        return true;

    /* Discriminant of 3k² – 2k – x = 0 : Δ = 4 + 12x */
    seq_t D = 4;
    D += 12 * x;

    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD != D)          /* Δ must be a perfect square */
        return false;

    /* k = (2 + sqrtΔ) / 6 must be integral */
    seq_t num = sqrtD + 2;
    return (num % 6 == 0);
}

/* The sequence A117082 */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many qualifying octagonal numbers seen */

    for (idx_t k = 0; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t oct   = k_seq * (3 * k_seq - 2); /* O_k = k(3k‑2) */

        seq_t ds = digit_sum(oct);            /* digit sum of O_k */
        if (is_octagonal(ds)) {
            if (count == n)                 /* the required element found */
                return oct;
            ++count;
        }
    }
}
