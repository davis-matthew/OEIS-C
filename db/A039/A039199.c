/*
 * A039199  Numbers whose base-11 representation has the same number of 5's
 *          and 10's.
 * https://oeis.org/A039199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-11 representation contains an equal
   number of 5's and 10's. This implementation searches for the nth
   such number by testing each integer in increasing order. */

seq_t a(idx_t n)
{
    idx_t found = 0;      /* how many qualifying numbers have been seen */
    seq_t cand   = 0;     /* current candidate integer */

    for (;;) {
        seq_t x = cand;        /* work on a copy of the candidate */
        unsigned cnt5  = 0;     /* count of digit 5 in base-11 */
        unsigned cnt10 = 0;     /* count of digit 10 in base-11 */

        /* Count digits of x in base 11 */
        while (x != (seq_t)0) {
            seq_t digit = x % (seq_t)11;
            if (digit == (seq_t)5)
                ++cnt5;
            else if (digit == (seq_t)10)
                ++cnt10;
            x /= (seq_t)11;
        }

        /* For x == 0, the counts cnt5 and cnt10 remain 0, which is valid */
        if (cnt5 == cnt10) {
            ++found;
            if (found == n)
                return cand;
        }

        ++cand;
    }
}
