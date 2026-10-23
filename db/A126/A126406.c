/*
 * A126406  Number of base 19 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑19 n‑digit numbers (allowing leading zero)
 *            whose adjacent digits differ by at most two. This is implemented
 *            via a simple dynamic programming that counts all length‑n digit
 *            strings satisfying |d[i]−d[i+1]| ≤ 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1 by OEIS offset */
    if (n == 0U) return (seq_t)1;

    /* The alphabet of digits is 0 … 18. */
    enum { DIGITS = 19 };

    /* cur[j] holds the number of length‑k strings that end with digit j,
       after we have processed k digits.  Initially k = 1. */
    seq_t cur[DIGITS];
    for (int j = 0; j < DIGITS; ++j)
        cur[j] = (seq_t)1;           /* for n = 1 all 19 digits are allowed */

    /* Incrementally build up to length n.  For each new digit we apply the
       adjacency restriction |prev‑new| ≤ 2. */
    if (n > 1U) {
        seq_t next[DIGITS];
        for (idx_t len = 2U; len <= n; ++len) {
            /* Clear next vector */
            for (int j = 0; j < DIGITS; ++j)
                next[j] = (seq_t)0;

            /* Transition from each previous digit to the next digit */
            for (int i = 0; i < DIGITS; ++i) {
                seq_t val = cur[i];
                if (val == (seq_t)0) continue;
                int start = (i >= 2) ? (i - 2) : 0;
                int end   = (i + 2 <= 18) ? (i + 2) : 18;
                for (int j = start; j <= end; ++j)
                    next[j] += val;
            }

            /* Move next into cur for the next iteration */
            for (int j = 0; j < DIGITS; ++j)
                cur[j] = next[j];
        }
    }

    /* The total number of admissible strings of length n is the sum
       of the counts for each possible ending digit. */
    seq_t total = (seq_t)0;
    for (int j = 0; j < DIGITS; ++j)
        total += cur[j];

    return total;
}
