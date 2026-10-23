/*
 * A031495  Numbers whose base-9 representation has one fewer 0 than 8's.
 * https://oeis.org/A031495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-9 representation has one more 8 than 0 */
/* The function scans sequentially and counts qualifying numbers. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, undefined for n=0 */

    idx_t  count = 0;
    seq_t  num   = 1;

    for (;; ++num)
    {
        seq_t  x   = num;
        unsigned c8 = 0, c0 = 0;

        /* count digits 0 and 8 in base‑9 */
        while (x > 0)
        {
            unsigned d = (unsigned)(x % 9);
            if (d == 8)  ++c8;
            else if (d == 0) ++c0;
            x /= 9;
        }

        if (c8 == c0 + 1)          /* one more 8 than 0 */
        {
            ++count;
            if (count == n) return num;
        }
    }
}
