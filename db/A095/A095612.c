/*
 * A095612  a(n) = 111 written in base n.
 * https://oeis.org/A095612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 111 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base 1 (unary) */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 111; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert decimal 111 to base n, storing digits least‑to‑most */
    seq_t val = 111;
    seq_t digits[12];
    idx_t k = 0;
    while (val > 0) {
        digits[k++] = val % ((seq_t)n);
        val = val / ((seq_t)n);
    }

    /* Concatenate the decimal representations of those digits */
    seq_t res = 0;
    for (idx_t i = k; i > 0; --i) {
        seq_t d = digits[i - 1];
        if (d < 10) {
            res = res * 10 + d;
        } else {
            /* d ≥ 10 → has at least two decimal digits */
            seq_t tmp = d;
            seq_t dec[5];
            idx_t len = 0;
            while (tmp > 0) {
                dec[len++] = tmp % 10;
                tmp = tmp / 10;
            }
            for (idx_t j = len; j > 0; --j) {
                res = res * 10 + dec[j - 1];
            }
        }
    }
    return res;
}
