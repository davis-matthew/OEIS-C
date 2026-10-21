/*
 * A095490  a(n) = 49 written in base n.
 * https://oeis.org/A095490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of decimal 49 in base n, concatenated as
 *            decimal digits of the base‑n representation
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle base 1 (unary) specially: 49 ones. */
    if (n == (idx_t)1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 49; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    seq_t val = (seq_t)49;
    seq_t n_seq = (seq_t)n;
    /* Collect base‑n digits, least significant first. */
    int dlim[64];
    int dcnt = 0;
    while (val > (seq_t)0) {
        int d = (int)(val % n_seq);
        dlim[dcnt++] = d;
        val = val / n_seq;
    }

    /* Assemble the concatenated decimal number. */
    seq_t res = 0;
    for (int k = dcnt - 1; k >= 0; --k) {
        int d = dlim[k];
        unsigned ndig;
        if (d < 10)          ndig = 1;
        else if (d < 100)    ndig = 2;
        else if (d < 1000)   ndig = 3;
        else if (d < 10000)  ndig = 4;
        else                 ndig = 5;      /* n ≤ 10 000 so d < 10 000 */
        seq_t mul = pow10((idx_t)ndig);
        res = res * mul + (seq_t)d;
    }
    return res;
}
