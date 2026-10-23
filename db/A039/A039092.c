/*
 * A039092  Numbers whose base-9 representation has the same number of 2's and
 *          4's.
 * https://oeis.org/A039092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute-force enumeration of numbers whose base‑9 representation
   contains an equal number of 2's and 4's (OEIS A039092) */
seq_t a(idx_t n)
{
    if (n == 0) /* offset 1, but guard against zero */
        return 0;

    idx_t target = n;          /* number of terms to find */
    seq_t x = 0;               /* current integer to test */
    idx_t count = 0;           /* how many valid numbers seen */

    while (count < target) {
        /* Count 2's and 4's in base‑9 of x */
        unsigned c2 = 0, c4 = 0;
        seq_t y = x;
        do {
            unsigned digit = (unsigned)(y % 9);
            if (digit == 2)
                ++c2;
            else if (digit == 4)
                ++c4;
            y /= 9;
        } while (y != 0);

        if (c2 == c4) {
            ++count;
            if (count == target)
                return x;
        }
        ++x;
    }
    return x; /* unreachable, but keeps compiler happy */
}
