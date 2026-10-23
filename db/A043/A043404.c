/*
 * A043404  Numbers whose base-7 representation contains exactly four 2's.
 * https://oeis.org/A043404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-7 representation contains exactly four 2's
 * (computed by brute‑force search). */
static unsigned count_twos(seq_t value)
{
    unsigned cnt = 0;
    while (value) {
        if ((value % 7) == 2) {
            ++cnt;
            if (cnt > 4)   /* early exit if too many 2's */
                return cnt;
        }
        value /= 7;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n==0, but return 0 harmlessly */
        return 0;

    idx_t found = 0;
    seq_t num   = 0;   /* we start counting from 1 */

    for (;;) {
        ++num;
        if (count_twos(num) == 4) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
