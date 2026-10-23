/*
 * A144483  A144481(4n-3).
 * https://oeis.org/A144483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value determined by r = (4*n-3) mod 9 r = 0 or 8 -> 6 r =
 *            1 or 4 or 7 -> 5 r = 2 or 6 -> 3 r = 3 or 5 -> 0
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute r = (4*n - 3) mod 9 */
    idx_t t = n % 9;                 /* t in [0,8] */
    seq_t r = ((seq_t)4 * t + 6) % 9; /* 6 ≡ -3 (mod 9) */

    /* map r to the sequence value */
    seq_t res;
    if (r == 0 || r == 8)           res = 6;
    else if (r == 1 || r == 4 || r == 7) res = 5;
    else if (r == 2 || r == 6)      res = 3;
    else                            /* r == 3 or r == 5 */
        res = 0;

    return res;
}
