/*
 * A095627  a(n) = 118 written in base 10 - n.
 * https://oeis.org/A095627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by writing 118 in base (10‑n).
 *            For n=0..9 the base ranges from 10 down to 1 (unary)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Sequence stops at n=9; for larger n return 0 */
    if (n > 9)
        return (seq_t)0;

    int base = 10 - (int)n;
    seq_t result = (seq_t)0;

    if (base >= 2) {
        /* Convert 118 to the given base, collect digits, then assemble as decimal */
        int digits[32];
        int dcnt = 0;
        int temp = 118;
        while (temp > 0) {
            digits[dcnt++] = temp % base;
            temp /= base;
        }
        /* digits are in reverse order; assemble from most significant to least */
        for (int i = dcnt - 1; i >= 0; --i) {
            result = result * (seq_t)10 + (seq_t)digits[i];
        }
    } else { /* base == 1 : unary representation */
        for (int i = 0; i < 118; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
    }

    return result;
}
