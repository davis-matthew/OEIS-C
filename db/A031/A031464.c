/*
 * A031464  Numbers whose base-4 representation has 3 more 0's than 3's.
 * https://oeis.org/A031464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth non‑negative integer whose base‑4 representation
 * contains exactly three more 0's than 3's.  (Computed by brute‑force
 * enumeration of integers, testing the digit counts.)
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but 0 is invalid */
    idx_t target = n;                /* number of matches we need */
    idx_t count  = 0;                /* how many matches found so far */

    seq_t x = 0;                     /* current integer to test */

    while (count < target) {
        seq_t tmp = x;
        int c0 = 0, c3 = 0;

        /* Special case: base‑4 representation of 0 is "0" */
        if (tmp == (seq_t)0) {
            c0 = 1;  /* only one digit, which is 0 */
        } else {
            while (tmp > (seq_t)0) {
                int digit = (int)(tmp % (seq_t)4);
                if (digit == 0) c0++;
                else if (digit == 3) c3++;
                tmp /= (seq_t)4;
            }
        }

        if (c0 - c3 == 3) {          /* satisfy the required difference */
            count++;
            if (count == target) {
                return x;            /* nth number found */
            }
        }

        x++;
    }

    /* Should never reach here for valid n */
    return (seq_t)-1;
}
