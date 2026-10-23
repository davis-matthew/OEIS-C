/*
 * A126516  Number of base 21 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A126516: number of base‑21 n‑digit numbers
   (allowing leading zeros) whose adjacent digits differ by at most 4. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* empty string */

    /* dp_cur[d] = number of sequences of current length ending with digit d */
    seq_t dp_cur[21];
    seq_t dp_next[21];
    int d, offset;

    /* length 1: each digit can start a sequence */
    for (d = 0; d < 21; ++d) dp_cur[d] = 1;

    /* iterate lengths 2 … n */
    for (idx_t len = 1; len < n; ++len) {
        /* clear dp_next */
        for (d = 0; d < 21; ++d) dp_next[d] = 0;

        /* transition from current digits */
        for (d = 0; d < 21; ++d) {
            seq_t val = dp_cur[d];
            if (val == 0) continue;
            for (offset = -4; offset <= 4; ++offset) {
                int d2 = d + offset;
                if (d2 >= 0 && d2 < 21)
                    dp_next[d2] += val;
            }
        }

        /* move to next length */
        for (d = 0; d < 21; ++d) dp_cur[d] = dp_next[d];
    }

    /* sum counts for all possible last digits */
    seq_t total = 0;
    for (d = 0; d < 21; ++d) total += dp_cur[d];
    return total;
}
