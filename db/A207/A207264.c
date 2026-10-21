/*
 * A207264  Number of nX3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays avoiding horizontally 000 and
 *            011, vertically 011 and 101. Empirical recurrence (for n>15):
 *            a(n) = 5*a(n-1) -9*a(n-2) +12*a(n-3) -22*a(n-4) +25*a(n-5)
 *            -16*a(n-6) +21*a(n-7) -17*a(n-8) +a(n-9) -6*a(n-10) +6*a(n-11)
 *            +a(n-12) +2*a(n-13) -2*a(n-14) Initial values: a...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 55, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed initial 15 terms */
    static const seq_t init[15] = {
        (seq_t)6, (seq_t)36, (seq_t)82, (seq_t)193, (seq_t)488,
        (seq_t)1087, (seq_t)2305, (seq_t)4932, (seq_t)10416,
        (seq_t)21716, (seq_t)45333, (seq_t)94798, (seq_t)198291,
        (seq_t)415803, (seq_t)874740
    };

    /* Coefficients for the recurrence, from a(n-1) down to a(n-14) */
    const int coeff[14] = {
        +5, -9, +12, -22, +25, -16, +21, -17,
        +1, -6, +6, +1, +2, -2
    };

    /* For n<=15, simply return the pre‑computed value */
    if (n <= 15) {
        return init[n - 1];
    }

    /* Buffer holds the most recent 14 values:
       buf[0] = a(curr-14), … , buf[13] = a(curr-1)   */
    seq_t buf[14];
    /* Initialize buffer with a(2)..a(15) */
    for (idx_t i = 0; i < 14; ++i) {
        buf[i] = init[i + 1];
    }

    /* Compute successive terms up to n */
    for (idx_t curr = 16; curr <= n; ++curr) {
        seq_t pos = 0, neg = 0;
        /* Compute the linear combination */
        for (int j = 0; j < 14; ++j) {
            seq_t term = buf[13 - j];
            int c = coeff[j];
            if (c > 0) {
                pos += (term * (seq_t)c);
            } else {
                neg += (term * (seq_t)(-c));
            }
        }
        seq_t new_val = pos - neg;

        /* Shift buffer left and insert the new term */
        for (int j = 0; j < 13; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[13] = new_val;
    }

    return buf[13];
}
