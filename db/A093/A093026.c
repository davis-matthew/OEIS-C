/*
 * A093026  Indices of terms in A093019 with value 6.
 * https://oeis.org/A093026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   numbers whose Luhn‑mod‑10 check digit is 6
 * Range:     u8 n <= 25, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the nth integer whose Luhn check digit equals 6.  This
       algorithm scans all integers sequentially; the density of such
       integers is roughly 1/10, so for n≈10000 at most a few hundred
       thousand iterations are required, well within the time limits.   */

    idx_t current = 1;     /* integer being tested */
    idx_t found   = 0;     /* count of integers satisfying the condition */

    while (1) {
        /* Compute the Luhn check digit for 'current'. */
        idx_t temp = current;
        int   sum  = 0;
        int   pos  = 1;       /* position of the digit from the right, 1‑based */

        while (temp != 0) {
            int digit = temp % 10;
            if (pos & 1) {   /* odd position: weight 2 */
                int val = digit * 2;
                if (val > 9) val -= 9;
                sum += val;
            } else {         /* even position: weight 1 */
                sum += digit;
            }
            temp /= 10;
            pos++;
        }

        int check = (10 - (sum % 10)) % 10;
        if (check == 6) {
            found++;
            if (found == n)
                return (seq_t)current;
        }
        current++;
    }
}
