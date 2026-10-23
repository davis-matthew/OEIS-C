/*
 * A039203  Numbers whose base-11 representation has the same number of 6's
 *          and 10's.
 * https://oeis.org/A039203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose base‑11 representation has the same number of 6's
 *            and 10's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms found so far   */
    seq_t x = 0;              /* current candidate number       */

    while (1) {
        /* test whether x satisfies the condition   */
        seq_t y = x;
        int  c6  = 0, c10 = 0;

        if (y == 0) {                     /* the digit 0 has no 6 or 10 */
            /* c6 and c10 remain 0           */
        } else {
            while (y > 0) {
                seq_t d = y % (seq_t)11;   /* extract next base‑11 digit   */
                if (d == 6)  c6++;
                else if (d == 10) c10++;
                y /= (seq_t)11;
            }
        }

        if (c6 == c10) {                  /* satisfies the equality       */
            count++;
            if (count == n)
                return x;
        }

        x++;                              /* try next integer            */
    }
}
