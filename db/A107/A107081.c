/*
 * A107081  Add 12, 3, 45, 67, 89 (note the difference sequence uses each
 *          digit once). Start with 1 for no particular reason; would seem
 *          more natural to start with 0.
 * https://oeis.org/A107081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, dumb, fini, full, nonn
 * Formula:   a(n) = 1 + floor(n/5)*216 + {0,12,15,60,127}[n mod 5]
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=a(n-1)+d[(n-1)%5], d={12,3,45,67,89} */

seq_t a(idx_t n)
{
    /* Sum of a complete cycle of differences */
    static const seq_t cycle_sum = (seq_t)216;

    /* Prefix sums of the first r differences */
    static const seq_t prefix[5] = {
        (seq_t)0,   /* r = 0 */
        (seq_t)12,  /* r = 1  (12)            */
        (seq_t)15,  /* r = 2  (12+3)          */
        (seq_t)60,  /* r = 3  (12+3+45)       */
        (seq_t)127  /* r = 4  (12+3+45+67)    */
    };

    /* Number of complete cycles */
    seq_t q = (seq_t)(n / 5);
    idx_t r = n % 5;

    /* a(n) = 1 + q*cycle_sum + prefix[r] */
    return (seq_t)1 + q * cycle_sum + prefix[r];
}
