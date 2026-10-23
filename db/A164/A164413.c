/*
 * A164413  Number of binary strings of length n with no substrings equal to
 *          0000, 0001 or 1001.
 * https://oeis.org/A164413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(((1 - sqrt(5))/2)^n + ((1 + sqrt(5))/2)^n) for n>4
 *            a(n) = a(n-1) + a(n-2) for n>6 with a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=8, a(4)=13, a(5)=22, a(6)=36
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base_vals[7] = {1, 2, 4, 8, 13, 22, 36};

seq_t a(idx_t n)
{
    /* base cases */
    if (n <= 6) {
        return base_vals[n];
    }

    /* recurrence a(n)=a(n-1)+a(n-2) for n>6 */
    seq_t prev2 = base_vals[5]; /* a(5) */
    seq_t prev1 = base_vals[6]; /* a(6) */
    seq_t cur;

    for (idx_t i = 7; i <= n; ++i) {
        cur   = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
