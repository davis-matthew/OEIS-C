/*
 * A098354  Multiplication table of the powers of 2 read by antidiagonals.
 * https://oeis.org/A098354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^k where k is the index of the diagonal containing n.
 *            Diagonal lengths increase as 1,2,3,4,… so the smallest m with
 *            m(m+1)/2 ≥ n gives k = m+1
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find minimal m with triangular number T_m = m(m+1)/2 ≥ n.
       For n ≥ 1 this loop iterates at most about √(2 n) times. */
    idx_t m = 1;
    idx_t tri = 1;   /* T_1 */

    while (tri < n) {
        ++m;
        tri += m;    /* T_m = T_{m-1} + m */
    }
    idx_t exp = m + 1;          /* exponent for 2^exp */

    /* Compute 2^exp.  Using a left‑shift works for all supported
       seq_t types (fixed‑width or big integer) and avoids pulling in
       external utility functions. */
    return ((seq_t)1 << exp);
}
