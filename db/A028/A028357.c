/*
 * A028357  Partial sums of A028356.
 * https://oeis.org/A028357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 + 15 * (n/6) + {0,2,5,9,12,14}[n%6] */
/* This implements the partial sums of the sequence whose
   consecutive differences repeat the pattern 2,3,4,3,2,1. */

seq_t a(idx_t n)
{
    /* Prefix sums of one period of length 6: 0,2,5,9,12,14 */
    static const seq_t pref[6] = {0, 2, 5, 9, 12, 14};

    idx_t q = n / 6;
    idx_t r = n % 6;

    return (seq_t)1 + (seq_t)15 * (seq_t)q + pref[r];
}
