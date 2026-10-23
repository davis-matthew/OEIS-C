/*
 * A039536  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 10's.
 * https://oeis.org/A039536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm for OEIS A039536: numbers whose base‑12 representation has the same nonzero number of 4's and 10's. */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    idx_t found = 0;
    seq_t m = 1;

    while (found < n) {
        seq_t tmp = m;
        int cnt4  = 0, cnt10 = 0;

        while (tmp > 0) {
            int d = (int)(tmp % 12);
            if (d == 4)     cnt4++;
            else if (d == 10) cnt10++;
            tmp /= 12;
        }

        if (cnt4 > 0 && cnt4 == cnt10) {
            found++;
            if (found == n) return m;
        }
        m++;
    }
    return m; /* unreachable */
}
