/*
 * A039272  Numbers whose base-12 representation has the same number of 9's
 *          and 10's.
 * https://oeis.org/A039272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   The n‑th nonnegative integer whose base‑12 representation
 *            contains an equal number of digits 9 and 10
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t  count = 0;   /* how many qualifying numbers we have found */
    seq_t  num   = 0;   /* current number being tested */

    while (count < n) {
        seq_t  x = num;
        unsigned int c9  = 0;
        unsigned int c10 = 0;

        /* Count the 9's and 10's in the base‑12 representation of x */
        while (x > 0) {
            unsigned int digit = (unsigned int)(x % 12);
            if (digit == 9)  c9++;
            else if (digit == 10) c10++;
            x /= 12;
        }

        if (c9 == c10) {          /* it qualifies */
            count++;
            if (count == n)        /* found the n‑th one */
                break;
        }
        num++;                     /* try next integer */
    }
    return num;
}
