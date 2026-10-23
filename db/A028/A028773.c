/*
 * A028773  Nonsquares mod 60.
 * https://oeis.org/A028773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth positive integer that is NOT a quadratic residue
 *            modulo 60
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _nonr[48] = {
    2, 3, 5, 6, 7, 8, 10, 11, 12, 13,
    14, 15, 17, 18, 19, 20, 22, 23, 26, 27,
    28, 29, 30, 31, 32, 33, 34, 35, 37, 38,
    39, 41, 42, 43, 44, 46, 47, 48, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59
};

seq_t a(idx_t n)
{
    /* n ≥ 1; 0‑based index for periodic handling */
    idx_t idx = n - 1;
    idx_t q = idx / 48;          /* number of complete 48‑term blocks passed */
    idx_t r_index = idx % 48;    /* position within the block (0‑based) */
    seq_t period_add = (seq_t)(q * 60); /* 60 per block, cast after multiplication */
    return _nonr[(size_t)r_index] + period_add;
}
