/*
 * A095618  a(n) = 114 written in base n.
 * https://oeis.org/A095618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 114 written in base n, interpreted as a decimal integer
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base‑1 special case: 114 ones. */
    if (n <= 1) {
        seq_t res = 0;
        for (int i = 0; i < 114; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* 114 written in base n. */
    unsigned long long lim = (unsigned long long)n;
    unsigned long long tmp = 114ULL;
    int digits[64];
    int len = 0;

    while (tmp > 0) {
        unsigned long long r = tmp % lim;
        digits[len++] = (int)r;
        tmp /= lim;
    }

    /* Convert the digit array into a decimal integer. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t pow = (seq_t)1;
        unsigned int t = (unsigned int)d;
        do {
            pow *= (seq_t)10;
            t /= 10;
        } while (t > 0);
        res = res * pow + (seq_t)d;
    }
    return res;
}
