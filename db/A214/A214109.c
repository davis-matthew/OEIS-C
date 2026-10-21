/*
 * A214109  Number of 0..3 colorings on an nX6 array circular in the 6
 *          direction with new values 0..3 introduced in row major order.
 * https://oeis.org/A214109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A214109
 *
 *  Formula:  a(n) = 200*a(n-1) - 5769*a(n-2) + 11744*a(n-3)
 *                                   + 43057*a(n-4) - 89856*a(n-5) + 40625*a(n-6)
 *
 *  First six terms are obtained from the generating function
 *  and are incorporated in the recurrence via the array
 *  `new_val`.  The implementation below works for any
 *  seq_t and idx_t defined by the build system.
 */

seq_t a(idx_t n)
{
    /* For an undefined argument (n==0) return 0.
     * All valid values start at n==1.
     */
    if (n == 0) return (seq_t)0;

    /* Buffer holding the last six values:
     *   buf[0] = a(i-1), buf[1] = a(i-2), ..., buf[5] = a(i-6)
     *   Initially all zero (equivalent to a(0)=0).
     */
    seq_t buf[6] = {0, 0, 0, 0, 0, 0};

    /* N_i are the coefficients of the numerator of the generating function.
     * They are used only for the first six iterations.
     */
    static const long long N[7] = {0, -31, 1206, -5144, -5174, 42107, -28260};

    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)200 * buf[0];

        if (i >= 2) val -= (seq_t)5769 * buf[1];
        if (i >= 3) val += (seq_t)11744 * buf[2];
        if (i >= 4) val += (seq_t)43057 * buf[3];
        if (i >= 5) val -= (seq_t)89856 * buf[4];
        if (i >= 6) val += (seq_t)40625 * buf[5];

        /* Apply the numerator correction for i <= 6 */
        if (i <= 6) {
            long long ncoeff = N[i];
            if (ncoeff >= 0)
                val -= (seq_t)ncoeff;       /* subtract positive constant */
            else
                val += (seq_t)(-ncoeff);   /* add absolute value of negative constant */
        }

        /* Shift the buffer: buf[1] <- buf[0], buf[2] <- buf[1], ... */
        for (int k = 5; k >= 1; --k) buf[k] = buf[k - 1];
        buf[0] = val;                       /* buf[0] now holds a(i) */
    }

    return buf[0];   /* a(n) after the loop */
}
