/*
 * A027662  a(n)/1000 gives sqrt(n) to 3 places after the decimal point.
 * https://oeis.org/A027662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/*
 * Formula: a(n) = round(√n * 1000)
 * The function returns the nearest integer to √n * 1000.
 * It works for any seq_t type (arbitrary‑precision or fixed size)
 * and for any idx_t value n ≥ 1.
 */
seq_t a(idx_t n)
{
    /* target = n * 1,000,000   (because √(n*10^6) = √n * 1000) */
    seq_t target = (seq_t)n * ((seq_t)1000000);

    /* q = floor(√target) */
    seq_t q = floor_sqrt(target);

    /* Determine whether to round up or down.
     * We round up iff target ≥ (q + 0.5)^2.
     * Equivalently: 4*target ≥ (2*q + 1)^2.
     */
    seq_t four_target = target * ((seq_t)4);
    seq_t two_q_plus1 = (q << 1) + 1;
    seq_t cmp = two_q_plus1 * two_q_plus1;

    if (four_target < cmp)
        return q;          /* round down */
    else
        return q + 1;      /* round up (ties rounded up) */
}
