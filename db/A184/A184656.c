/*
 * A184656  floor(nr+h), where r=(1+sqrt(5))/2, h=-1/2; complement of A184657.
 * https://oeis.org/A184656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor((n-1 + n*sqrt(5)) / 2)
 *           equivalently a(n) = floor(n*phi - 1/2),  phi = (1+sqrt(5))/2
 */

#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

seq_t a(idx_t n) {
    /* n ≥ 1 (offset 1).  Behaviour for n=0 is undefined but safe. */
    seq_t nn = (seq_t)n;

    /* Compute floor(sqrt(5*n*n)) exactly using integer arithmetic. */
    seq_t prod = nn * nn * (seq_t)5;
    seq_t s = floor_sqrt(prod);

    /* Return floor((n-1 + s) / 2).  All operations are integer. */
    return ((nn - (seq_t)1) + s) / (seq_t)2;
}
