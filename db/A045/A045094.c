/*
 * A045094  Numbers whose base-4 representation contains exactly one 1 and
 *          four 2's.
 * https://oeis.org/A045094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-4 representation contains exactly one 1 and
 *            four 2's. Implementation: iterate integers, test the base‑4
 *            digit counts
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t  found = 0;          /* how many terms we have found */
    seq_t  val    = 0;          /* current integer candidate */

    for (;;) {
        val++;                  /* check next integer */

        seq_t   tmp = val;
        unsigned ones = 0;      /* count of digit 1 */
        unsigned twos = 0;      /* count of digit 2 */

        /* count digits in base 4, stop early if counts exceed limits */
        while (tmp) {
            unsigned d = (unsigned)(tmp % 4);   /* remainder 0..3 */
            tmp /= 4;

            if (d == 1) {
                ones++;
                if (ones > 1) break;           /* too many 1's */
            } else if (d == 2) {
                twos++;
                if (twos > 4) break;           /* too many 2's */
            }
        }

        /* if we exited early, counts were out of range */
        if (tmp == 0 && ones == 1 && twos == 4) {
            found++;
            if (found == n) return val;      /* nth term found */
        }
    }
}
