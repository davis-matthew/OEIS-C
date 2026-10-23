/*
 * A043492  Numbers having four 0's in base 10.
 * https://oeis.org/A043492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th smallest integer whose decimal representation
 *            contains exactly four zeros. The function constructs the number
 *            by combinatorial ranking
 * Range:     u8 none, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is outside the sequence; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /*--------------------------------------------------------------------
     * 1.  Find the decimal length L of the n‑th number.
     *--------------------------------------------------------------------*/
    idx_t        L = 5;          /* every term has at least 5 digits */
    seq_t cumulative = 0;

    for (;;)
    {
        /* Count of L‑digit numbers with exactly four zeros:
           C(L‑1,4) * 9^(L‑4)                                   */
        seq_t count = 1;

        /* C(L‑1,4) */
        for (int i = 0; i < 4; ++i)
            count = count * (seq_t)(L - 1 - i) / (i + 1);

        /* 9^(L‑4) */
        seq_t pow9 = 1;
        for (idx_t i = 0; i < L - 4; ++i)
            pow9 *= (seq_t)9;

        count *= pow9;

        if (cumulative + count >= n)
            break;               /* L is the required length */

        cumulative += count;
        ++L;
    }

    /*--------------------------------------------------------------------
     * 2.  Determine the position of the desired number among all L‑digit
     *     numbers (0‑based offset) and construct it digit by digit.
     *--------------------------------------------------------------------*/
    seq_t offset = n - cumulative - 1;   /* 0‑based index within length L */
    seq_t value  = 0;

    int zeros_left = 4;   /* still to place in the remaining positions */

    for (int pos = 1; pos <= (int)L; ++pos)
    {
        int start_digit = (pos == 1 ? 1 : 0);   /* leading digit cannot be 0 */

        for (int d = start_digit; d <= 9; ++d)
        {
            int z_left = zeros_left - (d == 0);
            int rem_len = (int)L - pos;          /* positions still to fill */

            if (z_left < 0 || z_left > rem_len)
                continue;                       /* impossible with this digit */

            /* Count how many completions are possible after choosing digit d */
            seq_t ways = 1;

            /* Choose positions of the remaining zeros among the remaining places:
               C(rem_len, z_left)                                   */
            for (int i = 0; i < z_left; ++i)
                ways = ways * (seq_t)(rem_len - i) / (i + 1);

            /* Assign digits 1‑9 to the non‑zero positions:
               9^(rem_len - z_left)                               */
            seq_t pow9rem = 1;
            for (int i = 0; i < rem_len - z_left; ++i)
                pow9rem *= (seq_t)9;

            ways *= pow9rem;

            if (offset >= ways)
            {
                offset -= ways;   /* skip all numbers starting with this digit */
                continue;
            }

            /* Choose this digit as the current most significant digit */
            value = value * (seq_t)10 + (seq_t)d;
            zeros_left = z_left;
            break;                /* move to the next position */
        }
    }

    return value;
}
