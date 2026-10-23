/*
 * A047347  Numbers that are congruent to {0, 1, 4} mod 7.
 * https://oeis.org/A047347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) gives the n‑th non‑negative integer that is 0, 1 or 4 (mod
 *            7), with offset 1 (a(1)=0). The explicit piecewise formula is:
 *            if n ≡ 1 (mod 3): a(n) = 7·⌊(n+2)/3⌋ − 7 if n ≡ 2 (mod 3): a(n)
 *            = 7·⌊(n+1)/3⌋ − 6 if n ≡ 0 (mod 3): a(n) = 7·⌊n/3⌋ − 3 The
 *            function implements this directly in O(1) time
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    seq_t k, res;

    if (r == 1) {
        k   = ((seq_t)n + 2) / 3;
        res = ((seq_t)7 * k) - 7;
    } else if (r == 2) {
        k   = ((seq_t)n + 1) / 3;
        res = ((seq_t)7 * k) - 6;
    } else { /* r == 0 */
        k   = (seq_t)n / 3;
        res = ((seq_t)7 * k) - 3;
    }

    return res;
}
