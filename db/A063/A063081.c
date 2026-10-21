/*
 * A063081  Dimension of the space of weight 2n cusp forms for Gamma_0( 13 ).
 * https://oeis.org/A063081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-6) + 14 for n > 7, with the initial values a(1)=0,
 *            a(2)=3, a(3)=5, a(4)=7, a(5)=9, a(6)=13, a(7)=13. This uses the
 *            proven linear recurrence applicable to A063081
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Store the six initial values (a(1) .. a(7)). */
    static const seq_t init[8] = {0, 0, 3, 5, 7, 9, 13, 13};

    /* For the first seven indices just return the stored values. */
    if (n <= 7) {
        return init[n];
    }

    /* For n > 7 use the recurrence a(n) = a(n-6) + 14.
       We compute the number of steps k to jump back to an index
       <= 7, then add 14*k to the initial value at that position. */
    idx_t k = (n - 2) / 6;          /* number of full 6‑step jumps */
    idx_t base = n - 6 * k;        /* resulting index in [2,7] */

    return init[base] + (seq_t)14 * (seq_t)k;
}
