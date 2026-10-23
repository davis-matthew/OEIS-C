/*
 * A031494  Numbers whose base-9 representation has 2 more 0's than 8's.
 * https://oeis.org/A031494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A031494 – Numbers whose base‑9 representation
 * has two more 0's than 8's.
 *
 * The function scans successive integers and keeps those for which
 * count(0) == count(8) + 2.  It returns the n-th such integer
 * (with a(1)=81).
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */

    seq_t val = 81;                 /* first candidate */
    idx_t  found = 0;

    for (;; ++val) {
        seq_t temp = val;
        seq_t zeros = 0;
        seq_t eights = 0;

        /* count 0's and 8's in base‑9 representation */
        while (temp > 0) {
            seq_t digit = temp % 9;
            if (digit == 0) zeros++;
            else if (digit == 8) eights++;
            temp /= 9;
        }

        if (zeros == eights + 2) {
            ++found;
            if (found == n) return val;
        }
    }
}
