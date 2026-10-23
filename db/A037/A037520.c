/*
 * A037520  Base 3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0.
 * https://oeis.org/A037520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3 * a(n-1) + digit[n] with a(1) = 2 where digit[n] is the
 *            (n-1)th element of the periodic sequence {2,1,0}
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    /* period of base‑3 digits: 2,1,0 repeated */
    static const unsigned int period[3] = {2, 1, 0};

    for (idx_t i = 1; i <= n; ++i) {
        unsigned int d = period[(i - 1) % 3];
        val = val * 3 + d;
    }
    return val;
}
