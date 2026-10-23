/*
 * A045284  Numbers whose base-5 representation contains exactly one 2 and
 *          three 4's.
 * https://oeis.org/A045284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest integer whose base‑5 representation contains
 *            exactly one digit equal to 2 and exactly three digits equal to 4
 *            (and any other digits 0, 1, or 3)
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t x = 1; ; ++x)
    {
        seq_t y = x;
        idx_t c2 = 0, c4 = 0;
        while (y > 0)
        {
            seq_t d = y % 5;
            if (d == 2)
            {
                ++c2;
                if (c2 > 1) break;
            }
            else if (d == 4)
            {
                ++c4;
                if (c4 > 3) break;
            }
            y /= 5;
        }
        if (y == 0 && c2 == 1 && c4 == 3)
        {
            ++count;
            if (count == n) return x;
        }
    }
}
