/*
 * A200182  Number of -n..n arrays x(0..3) of 4 elements with zero sum and no
 *          two consecutive declines, no adjacent equal elements, and no
 *          element more than one greater than the previous (random base
 *          sawtooth pattern).
 * https://oeis.org/A200182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3k ) = 3*(k+1)^2 - 1 Formula: a(3k-1 ) = ((3k+2)^2 - 7)/3
 *            Formula: a(3k-2 ) = ((3k+1)^2 - 7)/3 (k ≥ 0) (All arithmetic is
 *            performed in the type seq_t.)
 * Range:     u8 n <= 24, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is not part of the sequence (offset 1), but return 0 for safety */
    if (n == 0u) return (seq_t)0;

    idx_t r = n % 3;
    idx_t q = n / 3;
    seq_t k, ans;

    switch (r) {
        case 0: /* n = 3k where k = q */
            k   = (seq_t)q;
            ans = 3 * (k + 1) * (k + 1) - 1;
            break;

        case 1: /* n = 3k-2 where k = q + 1 */
            k   = (seq_t)(q + 1);
            ans = ((3 * k + 1) * (3 * k + 1) - 7) / 3;
            break;

        case 2: /* n = 3k-1 where k = q + 1 */
            k   = (seq_t)(q + 1);
            ans = ((3 * k + 2) * (3 * k + 2) - 7) / 3;
            break;
    }
    return ans;
}
