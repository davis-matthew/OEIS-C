/*
 * A126369  Number of base 15 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n base‑15 digit sequences where adjacent
 *            digits differ by at most 1. Recurrence: let d[i] be counts of
 *            sequences ending with digit i. d[0..14]^(1) = 1 for all i. For
 *            k>1: d[i] = d[i] + (i>0? d[i-1]:0) + (i<14? d[i+1]:0) a(n) =
 *            sum_{i=0}^{14} d[i]
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)                        /* a(0) = 1 */
        return 1;

    /* DP tables: counts for each ending digit */
    seq_t cur[15];
    seq_t nxt[15];

    /* length 1: one sequence per digit */
    for (int i = 0; i < 15; ++i)
        cur[i] = 1;

    /* iterate for lengths 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < 15; ++i) {
            seq_t val = cur[i];
            if (i > 0)  val += cur[i - 1];
            if (i < 14) val += cur[i + 1];
            nxt[i] = val;
        }
        /* copy nxt to cur for next step */
        for (int i = 0; i < 15; ++i)
            cur[i] = nxt[i];
    }

    /* sum over all ending digits */
    seq_t result = 0;
    for (int i = 0; i < 15; ++i)
        result += cur[i];

    return result;
}
