/*
 * A010150  Continued fraction for sqrt(71).
 * https://oeis.org/A010150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 9729, u16 n <= 9729, u32 n <= 9729, u64 n <= 9729,
 *            u128 n <= 9729, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction for sqrt(71).
   a(0) = 8
   For n > 0: a(n) = {2, 2, 1, 7, 1, 2, 2, 16}[(n-1) mod 8] */

seq_t a(idx_t n)
{
    /* The period length is 8 (a power of two), so we can use & instead of % for speed. */
    static const seq_t period[8] = {2, 2, 1, 7, 1, 2, 2, 16};

    if (n == 0)
        return (seq_t)8;
    /* (n-1) is always non‑negative because n>0 here. */
    idx_t pos = (n - 1) & 7;   /* equivalent to (n-1) % 8 */
    return period[pos];
}
