/*
 * A032954  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 5265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A032954: numbers whose base‑4 representation has even
   digits at all odd positions (counted from the most significant digit). */

seq_t a(idx_t n)
{
    /* 0‑based counter of accepted numbers */
    idx_t count = 0;
    /* candidate integer to test */
    idx_t num = 1;

    while (count < n)
    {
        /* extract base‑4 digits of num in reverse order (least significant first) */
        unsigned char rev[128];
        idx_t tmp = num;
        idx_t dlen = 0;
        while (tmp)
        {
            rev[dlen++] = (unsigned char)(tmp & 3U);   /* tmp % 4 */
            tmp >>= 2U;                                /* tmp /= 4 */
        }
        if (dlen == 0) dlen = 1; /* (not needed for num>=1, but keeps the logic generic) */

        /* check that for every odd position (counting from 0 at the most
           significant digit) the digit is even (0 or 2). */
        int ok = 1;
        for (idx_t j = 0; j < dlen; ++j)
        {
            /* (dlen-1-j) is the position from the most significant digit */
            if (((dlen - 1 - j) & 1U) == 1U) /* odd position */
            {
                if ((rev[j] & 1U) == 1U) /* digit is odd */
                {
                    ok = 0;
                    break;
                }
            }
        }

        if (ok)
        {
            /* we found the next term of the sequence */
            ++count;
            if (count == n)
                return (seq_t)num;
        }

        ++num;
    }

    /* never reached */
    return (seq_t)0;
}
