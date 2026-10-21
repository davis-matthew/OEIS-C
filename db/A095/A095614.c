/*
 * A095614  a(n) = 112 written in base n.
 * https://oeis.org/A095614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 112 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* for base 1 the sequence is the unary expansion of 112,
       i.e. 112 consecutive '1' digits, which is a huge integer
       that only the arbitrary-precision build can represent. */
    if (n == (idx_t)1) {
        seq_t res = 0;
        /* append 112 digits '1' */
        for (idx_t i = 0; i < (idx_t)112; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* 112 decimal */
    const idx_t value = 112;
    idx_t v = value;

    /* buffer for digits in base n (size safe for all n >= 2) */
    int digits[64];
    int pos = 0;

    /* gather digits least‑significant first */
    while (v > 0) {
        digits[pos++] = v % n;
        v /= n;
    }
    if (pos == 0) {          /* should not happen because 112 > 0 */
        digits[pos++] = 0;
    }

    /* convert the base‑n representation to a decimal integer by
       concatenating the decimal forms of each digit. */
    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i) {
        seq_t d = digits[i];

        /* compute the number of decimal digits of d */
        idx_t len = 0;
        seq_t tmp = d;
        do {
            ++len;
            tmp /= (seq_t)10;
        } while (tmp > (seq_t)0);

        /* result = result * 10^len + d */
        seq_t pow10 = 1;
        for (idx_t j = 0; j < len; ++j) {
            pow10 *= (seq_t)10;
        }
        result = result * pow10 + d;
    }

    return result;
}
