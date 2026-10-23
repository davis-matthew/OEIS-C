/*
 * A126526  Number of base 31 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n sequences of digits 0…30 with
 *            |adjacent digits| ≤ 4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP implementation: counts walks of length n–1 on a 31‑vertex graph where edges connect
   vertices whose indices differ by at most 4.  The first digit may be any 0…30.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* the empty string */

    seq_t dp_prev[31];
    seq_t dp_curr[31];

    /* length 1: one way for each digit */
    for (int d = 0; d < 31; ++d) dp_prev[d] = (seq_t)1;

    if (n == 1) {
        seq_t sum = 0;
        for (int d = 0; d < 31; ++d) sum += dp_prev[d];
        return sum;
    }

    /* extend the string up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 31; ++d) {
            seq_t sum = 0;
            int low  = d - 4; if (low < 0)       low  = 0;
            int high = d + 4; if (high > 30) high = 30;
            for (int j = low; j <= high; ++j) sum += dp_prev[j];
            dp_curr[d] = sum;
        }
        for (int d = 0; d < 31; ++d) dp_prev[d] = dp_curr[d];
    }

    /* total number of strings of length n */
    seq_t result = 0;
    for (int d = 0; d < 31; ++d) result += dp_prev[d];
    return result;
}
