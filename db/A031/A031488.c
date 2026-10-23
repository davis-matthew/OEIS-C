/*
 * A031488  Numbers whose base-8 representation has one more 0 than 7's.
 * https://oeis.org/A031488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over integers and pick those with one more 0 than 7 in base‑8. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1;
    while (1) {
        seq_t tmp = candidate;
        unsigned cnt0 = 0, cnt7 = 0;
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 8);
            if (digit == 0) ++cnt0;
            else if (digit == 7) ++cnt7;
            tmp /= 8;
        }
        if (cnt0 == cnt7 + 1) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
