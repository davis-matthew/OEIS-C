/*
 * A095636  a(n) = 123 written in base n.
 * https://oeis.org/A095636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 123 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Base 1: unary representation of the decimal number 123. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 123; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    seq_t nseq = (seq_t)n;
    seq_t x = (seq_t)123;

    /* Store the base‑n digits of 123 in little‑endian order. */
    seq_t digits[16];
    int dcnt = 0;
    while (x != 0) {
        seq_t rem = x % nseq;
        digits[dcnt++] = rem;
        x = x / nseq;
    }

    /* Build the decimal number formed by concatenating those digits. */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        seq_t d = digits[i];
        /* d is always ≤ 123, so it fits in an unsigned int. */
        unsigned int dig = (unsigned int)d;
        /* Choose the appropriate power of 10 (at most 1000). */
        seq_t pow10 = (dig < 10 ? (seq_t)10
                         : (dig < 100 ? (seq_t)100
                                      : (seq_t)1000));
        res = res * pow10 + dig;
    }
    return res;
}
