/*
 * A206543  Period 10: repeat 1, 3, 5, 7, 9, 9, 7, 5, 3, 1.
 * https://oeis.org/A206543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Modd11(A204454(n))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204454(n) is the nth odd positive integer not divisible by 11, */
/* Modd11(x) is the sequence with period 22: 0,1,2,3,4,5,6,7,8,9,10,0,10,9,8,7,6,5,4,3,2,1 */
/* This implementation reduces the problem to a simple O(1) calculation. */

#include <stddef.h>   /* for size_t, not strictly needed but harmless */

seq_t a(idx_t n)
{
    /* index of n in the 10‑term pattern (0‑based) */
    idx_t idx = (n - 1) % 10;

    /* map idx to the value of the odd index modulo 11 */
    /* offset 0..4 -> 1..5, offset 5..9 -> 7..11 (mod 11) */
    idx_t d = (idx < 5) ? (idx + 1) : ((idx + 2) % 11);

    /* compute raw = (2*d – 1) mod 22, result in [0,21] */
    int raw = (int)(2 * d - 1);
    if (raw < 0)          /* handles the case d == 0 (raw == -1) */
        raw += 22;        /* now raw is in 0..21 */

    /* finally produce the Modd11 value in [0,10] */
    seq_t res = (raw <= 10) ? (seq_t)raw : (seq_t)(22 - raw);

    return res;
}
