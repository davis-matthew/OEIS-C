/*
 * A039574  Numbers whose base-6 representation has the same number of 0's,
 *          1's and 3's.
 * https://oeis.org/A039574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th integer whose base‑6 representation has equal counts
 *            of 0, 1, and 3. (Computed by a linear search.)
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; the loop stops when we have found n good numbers */
    idx_t found = 0;
    seq_t   val   = 1;          /* candidate integer to test */

    while (found < n) {
        seq_t tmp = val;
        int c0 = 0, c1 = 0, c3 = 0;

        /* Count 0s, 1s, and 3s in the base‑6 representation of val */
        if (tmp == 0) {          /* handle the unlikely case of val == 0 */
            c0 = 1;
        } else {
            while (tmp > 0) {
                unsigned r = (unsigned)(tmp % 6);
                if (r == 0)       c0++;
                else if (r == 1)  c1++;
                else if (r == 3)  c3++;
                tmp /= 6;
            }
        }

        if (c0 == c1 && c1 == c3) {
            ++found;              /* found a valid number */
        }
        ++val;                    /* test next integer */
    }

    return val - 1;               /* last accepted value */
}
