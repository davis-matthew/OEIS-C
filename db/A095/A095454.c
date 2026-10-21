/*
 * A095454  a(n) = 31 written in base n.
 * https://oeis.org/A095454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = "concatenation of the decimal digits of the base‑n
 *            representation of the decimal number 31". For n = 1 the base‑1
 *            representation is 31 ones
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 15, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 31 written in base n, converting the result to a decimal integer
   by concatenating the decimal representations of each base‑n digit. */
seq_t a(idx_t n)
{
    /* Special handling for base 1 (unary). */
    if (n == 1) {
        seq_t res = 0;
        /* 31 ones: 111…1 (31 digits). */
        for (idx_t i = 0; i < 31; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    seq_t num = 31;         /* the number to convert. */
    /* Store base‑n digits (least significant first). */
    idx_t digits[64];
    int cnt = 0;
    while (num > 0) {
        digits[cnt++] = (int)(num % n);
        num = num / n;
    }

    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        seq_t d = digits[i];

        /* Determine number of decimal digits of d. */
        seq_t len = 0;
        seq_t tmp = d;
        do {
            ++len;
            tmp = tmp / 10;
        } while (tmp > 0);

        /* Compute 10^len. */
        seq_t pow10 = 1;
        for (seq_t j = 0; j < len; ++j) {
            pow10 = pow10 * 10;
        }

        res = res * pow10 + d;
    }
    return res;
}
