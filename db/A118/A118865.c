/*
 * A118865  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 19.
 * https://oeis.org/A118865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n>1 a(n) = ((a(n-1) % 10^(d-1)) * 10 +
 *            a(n-1)/10^(d-1)) + 19, where d = digits(a(n-1))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS sequence A118865 */
seq_t a(idx_t n)
{
    seq_t v = (seq_t)1;          /* a(1) */
    if (n == (idx_t)1)
        return v;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* compute number of decimal digits of v */
        seq_t tmp = v;
        idx_t digits = (idx_t)0;
        do {
            tmp /= (seq_t)10;
            ++digits;
        } while (tmp > (seq_t)0);

        /* compute pow10 = 10^(digits-1) */
        seq_t pow10 = (seq_t)1;
        for (idx_t j = (idx_t)0; j < digits - (idx_t)1; ++j)
            pow10 *= (seq_t)10;

        /* move first digit to the end and add 19 */
        seq_t first = v / pow10;
        seq_t rest  = v % pow10;
        v = rest * (seq_t)10 + first + (seq_t)19;
    }
    return v;
}
