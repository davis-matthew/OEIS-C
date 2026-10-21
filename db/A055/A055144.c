/*
 * A055144  The first n digits of the juxtaposition of the base-3 numbers
 *          converted to decimal.
 * https://oeis.org/A055144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer obtained by reading the first n digits of the
 *            infinite base‑3 string formed by concatenating the base‑3
 *            representations of the positive integers, interpreted as a
 *            base‑3 number
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We generate the required digits one by one.  Each digit is a
       single decimal digit 0, 1, or 2.  As we receive a new digit d,
       we update the value:  value = value * 3 + d.  The value is kept
       in a seq_t so it can grow arbitrarily large. */
    idx_t pos = 0;           /* number of digits already consumed          */
    idx_t num = 1;           /* current positive integer to output in base 3 */
    seq_t value = 0;         /* running result (interpreted as base‑3)       */

    /* Temporary storage for the base‑3 digits of the current integer. */
    unsigned int digits[64];
    int dlen;

    while (pos < n) {
        /* Compute the base‑3 digits of 'num' into 'digits' (least
           significant first).  Since the largest number we will ever
           need is well below 3^64, 64 digits are sufficient. */
        uint64_t tmp = num;
        dlen = 0;
        do {
            digits[dlen++] = (unsigned int)(tmp % 3);
            tmp /= 3;
        } while (tmp > 0);

        /* Emit the digits from most significant to least.  Each
           digit contributes one more to the overall base‑3 number. */
        for (int i = dlen - 1; i >= 0 && pos < n; --i) {
            unsigned int d = digits[i];
            value = value * 3 + d;
            ++pos;
        }

        ++num;
    }

    return value;
}
