/*
 * A031481  Numbers whose base-6 representation has 2 fewer 0's than 5's.
 * https://oeis.org/A031481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer such that in base‑6 representation
 *            the count of digit 5 exceeds the count of digit 0 by exactly 2
 * Range:     u8 n <= 13, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1 with value 35.  We simply scan
       the natural numbers in increasing order and count the base‑6
       digits 0 and 5 until we have found the nth matching number. */
    idx_t found = 0;
    idx_t xval = 1;                    /* integer counter (idx_t) */

    for (;;) {
        seq_t x = (seq_t)xval;         /* current candidate in seq_t */
        unsigned zeros = 0, fives = 0;
        seq_t tmp = x;

        /* Count digits 0 and 5 in base 6. */
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 6);
            if (digit == 0)   zeros++;
            else if (digit == 5) fives++;
            tmp /= 6;
        }

        if (fives - zeros == 2) {
            ++found;
            if (found == n) return x;   /* found the desired term */
        }

        ++xval;                        /* next candidate */
    }
}
