/*
 * A162693  Strictly positive numbers k such that 30*k/(30+k) are integers.
 * https://oeis.org/A162693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 900/d - 30 where d is a positive divisor of 900 such that
 *            d < 30. The sequence is the ascending list of these values
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static array holding all 13 terms (computed once) */
    static seq_t arr[13];
    /* static counter of how many terms were generated */
    static idx_t sz = 0;

    /* compute the terms on first call */
    if (sz == 0) {
        idx_t cnt = 0;
        /* iterate over possible divisors from 29 down to 1;
           this yields the k-values in ascending order */
        for (idx_t d = 29; d > 0; --d) {
            if ((((idx_t)900) % d) == 0) {
                idx_t val = (((idx_t)900) / d) - 30;
                arr[cnt++] = (seq_t)val;
            }
        }
        sz = cnt;      /* sz will be 13 for all supported integer types */
    }

    /* return the n-th term if it exists, otherwise 0 */
    if (n >= 1 && n <= sz)
        return arr[n - 1];
    else
        return 0;
}
