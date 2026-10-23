/*
 * A047620  Numbers that are congruent to {0, 1, 2, 5} mod 8.
 * https://oeis.org/A047620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n-3 + sin(n*Pi/2)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sin(n*Pi/2) cycles: 1, 0, -1, 0 for n mod 4 = 1,2,3,0 respectively */
seq_t a(idx_t n)
{
    /* Compute sin(n*Pi/2) as an integer value */
    int r = (int)(n % 4);
    int sin_val;
    if (r == 1)
        sin_val = 1;
    else if (r == 3)
        sin_val = -1;
    else
        sin_val = 0;

    seq_t n_seq = (seq_t)n;
    /* Unsigned arithmetic wraps correctly for signed intermediate values */
    seq_t result = 2 * n_seq - 3 + (seq_t)sin_val;
    return result;
}
