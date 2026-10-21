/*
 * A095594  a(n) = 102 written in base n.
 * https://oeis.org/A095594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by writing 102 in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the integer corresponding to the expansion of 102 (decimal) in base n.
 * For base 1, the expansion is a unary string of 102 ones.
 * For n ≥ 2, we form the ordinary base‑n representation of 102 and then
 * concatenate the decimal strings of its digits (each digit is 0‑999).  The
 * result is returned as a seq_t.  The routine works for any idx_t and seq_t
 * type, using only integer arithmetic. */
seq_t a(idx_t n) {
    /* Base 1: unary representation of 102 → 102 ones */
    if (n == 1) {
        seq_t res = 0;
        /* 102 iterations – small constant time */
        for (idx_t i = 0; i < 102; ++i) {
            res = res * ((seq_t)10) + ((seq_t)1);
        }
        return res;
    }

    seq_t tmp = (seq_t)102;   /* value to be converted */
    /* digits in reverse order; 102 is small → at most 7 digits */
    seq_t digits[10];
    idx_t dcnt = 0;

    /* Extract digits of 102 in base n */
    while (tmp > 0) {
        seq_t digit = tmp % (seq_t)n;
        digits[dcnt++] = digit;
        tmp = tmp / (seq_t)n;
    }

    /* Build the concatenated decimal representation */
    seq_t res = 0;
    for (idx_t i = dcnt; i > 0; --i) {
        seq_t digit = digits[i - 1];
        /* Determine decimal length of 'digit' */
        int len = 1;
        if (digit >= 10) len = 2;
        if (digit >= 100) len = 3;   /* max digit is 102 for n>102 */

        /* pow10 = 10^len */
        seq_t pow10 = 1;
        for (int k = 0; k < len; ++k) pow10 *= 10;

        res = res * pow10 + digit;
    }

    return res;
}
