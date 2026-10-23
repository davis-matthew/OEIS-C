/*
 * A045297  Numbers whose base-5 representation contains exactly one 3 and one
 *          4.
 * https://oeis.org/A045297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045297: Numbers whose base‑5 representation contains exactly one 3 and one 4.
   This implementation scans integers from 1 upward and tests the condition. */
seq_t a(idx_t n)
{
    seq_t x = 1;
    idx_t cnt = 0;
    for (;; ++x)
    {
        seq_t y = x;
        int c3 = 0, c4 = 0;
        int ok = 1;
        while (y)
        {
            int d = (int)(y % 5);
            if (d == 3)
            {
                ++c3;
                if (c3 > 1) { ok = 0; break; }
            }
            else if (d == 4)
            {
                ++c4;
                if (c4 > 1) { ok = 0; break; }
            }
            y /= 5;
        }
        if (ok && c3 == 1 && c4 == 1)
        {
            ++cnt;
            if (cnt == n) return x;
        }
    }
    return 0; /* unreachable */
}
