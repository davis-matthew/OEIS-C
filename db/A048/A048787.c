/*
 * A048787  Write n in base 3 then rotate left one place.
 * https://oeis.org/A048787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Rotate left one place the base‑3 representation of n
 * Range:     u8 n <= 327, u16 n <= 9692, u32 n <= 9692, u64 n <= 9692,
 *            u128 n <= 9692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≥ 1: find the highest power of 3 ≤ n. */
    if (n == 0) return 0;                 /* safety for an unexpected 0 */
    idx_t t = 1;
    while (t <= n / 3)                     /* keep t*3 ≤ n, avoid overflow */
        t *= 3;

    /* Most significant base‑3 digit. */
    idx_t msd = n / t;

    /* Remaining part after removing the most significant digit. */
    idx_t rest = n - msd * t;              /* rest < t */

    /* Compute rotated value: rest*3 + msd.  Use seq_t for the small
       arithmetic to stay exact when the final value fits in seq_t. */
    seq_t rest_seq = (seq_t)rest;
    seq_t res = rest_seq * 3 + (seq_t)msd;
    return res;
}
