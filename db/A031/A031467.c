/*
 * A031467  Numbers whose base-4 representation has 2 fewer 0's than 3's.
 * https://oeis.org/A031467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑4 representation has two fewer 0's than 3's */
/* Implementation: Linear search over integers, counting digits in base 4. */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many valid numbers found */
    seq_t cur   = 0;          /* current integer being tested */

    while (count < n) {
        /* move to next integer */
        cur++;

        /* count zeros and threes in base‑4 representation of cur */
        seq_t temp = cur;
        int  cnt0  = 0;
        int  cnt3  = 0;
        while (temp != 0) {
            seq_t digit = temp % 4;
            if (digit == 0)          cnt0++;
            else if (digit == 3)     cnt3++;
            temp = temp / 4;
        }

        /* check the required condition */
        if (cnt3 - cnt0 == 2) {
            count++;
        }
    }

    return cur;
}
