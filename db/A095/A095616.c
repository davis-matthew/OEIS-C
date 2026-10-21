/*
 * A095616  a(n) = 113 written in base n.
 * https://oeis.org/A095616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 113 written in base n. For n = 1 a special treatment is
 *            used: a(1) is 113 ones. For n ≥ 2: digits of 113 in base n are
 *            extracted, then the decimal number that results from
 *            concatenating the decimal representations of these digits is
 *            returned
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case n = 1 */
    if (n == (idx_t)1) {
        seq_t res = (seq_t)0;
        for (idx_t i = (idx_t)0; i < (idx_t)113; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* extract base‑n digits of 113 (least significant first) */
    seq_t val   = (seq_t)113;
    seq_t base  = (seq_t)n;
    seq_t digits[130];
    int len = 0;
    while (val > (seq_t)0) {
        digits[len++] = val % base;
        val = val / base;
    }

    /* build the decimal number by appending each digit’s decimal form */
    seq_t res = (seq_t)0;
    for (int idx = len - 1; idx >= 0; --idx) {
        seq_t d = digits[idx];

        /* count decimal digits of d – at least 1, even for d == 0 */
        seq_t tmp = d;
        int k = 0;
        do {
            ++k;
            tmp = tmp / (seq_t)10;
        } while (tmp > (seq_t)0);

        /* compute 10^k */
        seq_t pow10 = (seq_t)1;
        for (int i = 0; i < k; ++i)
            pow10 *= (seq_t)10;

        res = res * pow10 + d;
    }

    return res;
}
