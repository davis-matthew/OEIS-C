/*
 * A043472  Numbers having four 4's in base 9.
 * https://oeis.org/A043472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly four 4's in base 9. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t candidate = 0;
    while (found < n) {
        candidate++;
        idx_t tmp = candidate;
        unsigned cnt = 0;
        while (tmp) {
            if (tmp % 9 == 4) cnt++;
            tmp /= 9;
        }
        if (cnt == 4) {
            found++;
        }
    }
    return (seq_t)candidate;
}
