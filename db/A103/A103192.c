/*
 * A103192  Trajectory of 1 under repeated application of the function n ->
 *          A102370(n).
 * https://oeis.org/A103192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n - 1 + 8*floor(n/4) (n ≥ 1)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The sequence is the trajectory of 1 under the function
       f(n) = n + 2, except when n ≡ 5 mod 16, in which case
       f(n) = n + 10.  The resulting numbers can be summed
       directly to give the closed form above. */
    idx_t q = n / 4;                /* floor(n/4) */
    seq_t part = (seq_t)2 * (seq_t)n;   /* 2*n  */
    part = part - 1;                 /* 2*n - 1 */
    part = part + (seq_t)8 * (seq_t)q; /* + 8*floor(n/4) */
    return part;
}
