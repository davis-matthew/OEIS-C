/*
 * A095460  a(n) = 34 written in base n.
 * https://oeis.org/A095460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34 written in base n (digits concatenated in decimal)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 11, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* Undefined for n=0, return 0 safely */

    if (n == 1) {                      /* Unary base: 34 ones */
        seq_t res = (seq_t)0;
        for (idx_t i = 0; i < 34; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    idx_t val = (idx_t)34;             /* The constant to convert */
    int digits[10];                    /* Enough for all cases (max 6 digits for base 2) */
    int dcnt = 0;

    while (val != (idx_t)0) {          /* Extract base‑n digits (LSB first) */
        digits[dcnt++] = (int)(val % n);
        val = val / n;
    }

    seq_t res = (seq_t)0;
    for (int i = dcnt - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t mult = (d < 10) ? (seq_t)10 : (seq_t)100;   /* Each digit is 1 or 2 decimal digits */
        res = res * mult + (seq_t)d;
    }

    return res;
}
