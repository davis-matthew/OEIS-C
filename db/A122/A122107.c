/*
 * A122107  Numbers of the form (1+2n^2+4n^3)/(n+10).
 * https://oeis.org/A122107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, sign
 * Formula:   a(n) = (1 + 2n^2 + 4n^3) / (n + 10)
 * Range:     i8 none, i16 n <= 1, i32 none, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined only for n = 1..8 (offset 1) */
    if (n < 1) return 0;
    const idx_t LEN = 8;
    if (n > LEN) return 0;                /* undefined beyond length */

    /* Divisors of 3799 (1, 29, 131, 3799) and their negatives */
    const seq_t pos_divs[4] = { 1, 29, 131, 3799 };
    seq_t vals[LEN];

    idx_t k = 0;
    for (idx_t i = 0; i < 4; ++i) {
        /* Positive divisor */
        seq_t d = pos_divs[i];
        seq_t m = d - 10;                         /* n value */
        seq_t num = 1 + 2 * m * m + 4 * m * m * m;
        seq_t val = num / d;                      /* exact division */
        vals[k++] = val;

        /* Negative divisor */
        seq_t dneg = -d;
        seq_t mneg = dneg - 10;
        seq_t num_neg = 1 + 2 * mneg * mneg + 4 * mneg * mneg * mneg;
        seq_t val_neg = num_neg / dneg;           /* exact division */
        vals[k++] = val_neg;
    }

    /* Sort the eight values in ascending order */
    for (idx_t i = 1; i < LEN; ++i) {
        seq_t key = vals[i];
        idx_t j = i;
        while (j > 0 && vals[j - 1] > key) {
            vals[j] = vals[j - 1];
            --j;
        }
        vals[j] = key;
    }

    return vals[n - 1];
}
