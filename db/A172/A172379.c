/*
 * A172379  Partial list of labels attached to the quiver of the cluster
 *          algebra A_4^4.
 * https://oeis.org/A172379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, less, uned
 * Formula:   a(n) = the n‑th 4‑digit number abc d (digits 1–9, strictly
 *            increasing) such that each consecutive difference is 2 or 3 and
 *            at most one of the differences equals 3. The sequence is finite
 *            and contains exactly 9 terms
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range (offset 1) */

    idx_t count = 0;

    for (idx_t a = 1; a <= 7; ++a)                     /* a ≤ 7 makes a+2 ≤ 9   */
    {
        idx_t max_b = a + 3;
        if (max_b > 9) max_b = 9;
        for (idx_t b = a + 2; b <= max_b; ++b)
        {
            idx_t d1 = b - a;                          /* first gap  (2 or 3) */

            idx_t max_c = b + 3;
            if (max_c > 9) max_c = 9;
            for (idx_t c = b + 2; c <= max_c; ++c)
            {
                idx_t d2 = c - b;                      /* second gap (2 or 3) */

                idx_t max_d = c + 3;
                if (max_d > 9) max_d = 9;
                for (idx_t d = c + 2; d <= max_d; ++d)
                {
                    idx_t d3 = d - c;                  /* third gap  (2 or 3) */

                    /* Reject any combination where a gap > 3 or more than one gap equals 3 */
                    if ((d1 > 3) || (d2 > 3) || (d3 > 3)) continue;

                    idx_t cnt3 = 0;
                    if (d1 == 3) ++cnt3;
                    if (d2 == 3) ++cnt3;
                    if (d3 == 3) ++cnt3;
                    if (cnt3 > 1) continue;

                    ++count;
                    if (count == n)
                    {
                        return ((seq_t)a * (seq_t)1000) +
                               ((seq_t)b * (seq_t)100) +
                               ((seq_t)c * (seq_t)10) +
                               (seq_t)d;
                    }
                }
            }
        }
    }

    /* n exceeds the number of valid terms (only 9 exist) */
    return 0;
}
