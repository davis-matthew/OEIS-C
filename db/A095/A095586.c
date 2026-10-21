/*
 * A095586  a(n) = 97 written in base n.
 * https://oeis.org/A095586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 97 written in base n, interpreted as a decimal integer.
   For n = 1, the unary representation consists of 97 ones.
   For n ≥ 2, 97 is converted to base n, and the resulting digit sequence
   (where each digit may take several decimal digits) is concatenated to
   form a decimal integer. */
seq_t a(idx_t n)
{
    if (n == 1) {
        seq_t res = (seq_t)1;
        for (idx_t i = 1; i < 97; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* Convert 97 to base n, store digits least‑significant first */
    idx_t tmp = (idx_t)97;
    int digits[10];
    int cnt = 0;
    while (tmp > 0) {
        digits[cnt++] = (int)(tmp % n);
        tmp /= n;
    }

    /* Build the decimal integer by concatenating the digit strings */
    seq_t result = (seq_t)0;
    for (int i = cnt - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t digit_val = (seq_t)d;

        /* Find number of decimal digits in d */
        int len = 1;
        if (d >= 10) {
            if (d >= 100) {
                if (d >= 1000) len = 4;
                else len = 3;
            } else len = 2;
        }
        seq_t mult = (seq_t)1;
        for (int k = 0; k < len; ++k)
            mult = mult * (seq_t)10;

        result = result * mult + digit_val;
    }
    return result;
}
