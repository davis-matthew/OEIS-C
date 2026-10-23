/*
 * A048903  Heptagonal hexagonal numbers.
 * https://oeis.org/A048903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(2) = 121771 a(3) = 12625478965 a(n) = 103682*a(n-1) -
 *            a(n-2) + 18144 (n ≥ 4) Recurrence uses only +, -, *; all
 *            intermediate values fit in seq_t and subtraction is performed on
 *            a larger value, so no underflow occurs
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; value for n=0 is unused */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)121771;
    if (n == 3) return (seq_t)12625478965;

    /* iterate from n = 4 up to the requested n, keeping only the last two terms */
    seq_t a_nm2 = (seq_t)1;          /* a(1)  */
    seq_t a_nm1 = (seq_t)121771;     /* a(2)  */
    seq_t a_n;                       /* will hold a(k) */

    for (idx_t k = 3; k <= n; ++k) {
        /* a_k = 103682*a_{k-1} - a_{k-2} + 18144 */
        a_n = a_nm1 * (seq_t)103682;   /* 103682 * a_{k-1}   */
        a_n -= a_nm2;                  /* subtract a_{k-2}   */
        a_n += (seq_t)18144;           /* add constant       */

        /* shift one step forward for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;   /* a(n) after loop */
}
