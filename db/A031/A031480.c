/*
 * A031480  Numbers whose base-6 representation has one fewer 0 than 5's.
 * https://oeis.org/A031480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑6 representation has one fewer 0 than 5's. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, behaviour for 0 is irrelevant */

    idx_t found = 0;          /* how many qualifying numbers have been seen */
    seq_t current = 0;        /* current number being tested */

    for (;; ++current) {
        idx_t tmp = (idx_t)current;   /* tmp operates on a native integer type */
        int zeros  = 0;
        int fives  = 0;

        /* Count zeros and fives in the base‑6 representation of current. */
        while (tmp > 0) {
            int d = tmp % 6;
            if (d == 0)   ++zeros;
            else if (d == 5) ++fives;
            tmp /= 6;
        }

        if (fives - zeros == 1) {
            ++found;
            if (found == n) return current;
        }
    }
}
