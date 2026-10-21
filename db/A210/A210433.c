/*
 * A210433  Natural numbers k such that floor(v) * ceiling(v)^2 = k, where v =
 *          k^(1/3).
 * https://oeis.org/A210433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210433
   Formula: a(n) = (n/2)^3                if n is even
             a(n) = ((n-1)/2) * ((n+1)/2)^2   if n is odd
   where n ≥ 1 and all divisions are ordinary integer divisions. */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    seq_t k = (seq_t)(n >> 1);

    if ((n & 1) == 0) {
        /* n even: a(n) = k^3 */
        return k * k * k;
    } else {
        /* n odd: a(n) = k * (k+1)^2 */
        seq_t kp1 = k + 1;
        seq_t kp1_sq = kp1 * kp1;
        return k * kp1_sq;
    }
}
