/*
 * A043010  Base-6 palindromes that start with 1.
 * https://oeis.org/A043010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth base‑6 palindrome that starts with the digit 1 in base 6
   (sequence A043010).  The enumeration follows the natural order of the
   palindromic numbers, starting with 1, 11_6, 101_6, 111_6, …   */
seq_t a(idx_t n)
{
    idx_t remaining = n;
    idx_t power = 1;          /* 6^(m-1), the size of each group for length m */

    for (idx_t m = 1; ; ++m)
    {
        /* --- odd‑length group (length = 2*m–1) --- */
        if (remaining <= power)
        {
            if (m == 1)          /* the single digit 1 */
                return (seq_t)1;

            idx_t idx = remaining - 1;      /* 0‑based index inside the group */
            idx_t dlen = m - 1;             /* number of free digits */
            unsigned char d[64];            /* hold the free digits */

            idx_t tmp = idx;
            for (idx_t pos = dlen; pos > 0; --pos)
            {
                d[pos-1] = (unsigned char)(tmp % 6);
                tmp /= 6;
            }

            seq_t v = 0;
            v = v * 6 + 1;                    /* first digit */
            for (idx_t i = 0; i < dlen; ++i)
                v = v * 6 + d[i];
            for (int id = (int)dlen - 2; id >= 0; --id)   /* mirror excluding centre */
                v = v * 6 + d[id];
            v = v * 6 + 1;                    /* last digit */
            return v;
        }
        remaining -= power;

        /* --- even‑length group (length = 2*m) --- */
        if (remaining <= power)
        {
            if (m == 1)          /* 11_6 */
                return (seq_t)7;

            idx_t idx = remaining - 1;
            idx_t dlen = m - 1;
            unsigned char d[64];

            idx_t tmp = idx;
            for (idx_t pos = dlen; pos > 0; --pos)
            {
                d[pos-1] = (unsigned char)(tmp % 6);
                tmp /= 6;
            }

            seq_t v = 0;
            v = v * 6 + 1;
            for (idx_t i = 0; i < dlen; ++i)
                v = v * 6 + d[i];
            for (int id = (int)dlen - 1; id >= 0; --id)     /* mirror all free digits */
                v = v * 6 + d[id];
            v = v * 6 + 1;
            return v;
        }
        remaining -= power;
        power *= 6;      /* advance to next set of lengths */
    }
}
