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
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

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
