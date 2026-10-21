/*
 * A224546  Number of (n+1) X 5 0..1 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * OEIS A224546 – Number of (n+1)×5 0..1 matrices with each 2×2 subblock idempotent
 *
 * Recurrence (n > 6):
 *   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5)
 *
 * Initial values:
 *   a(1)=48, a(2)=86, a(3)=125, a(4)=196, a(5)=294, a(6)=448
 *
 * The function below uses a constant‑time rolling window over the last five
 * computed terms, so it runs in O(n) time and constant space for any n ≥ 1.
 *********************************************************************/

seq_t a(idx_t n)
{
    /* Treat n=0 as out of range – offset is 1 */
    if (n == 0) {
        return 0;
    }

    /* Return the first six hard‑coded terms */
    if (n <= 6) {
        static const seq_t base[7] = {0, 48, 86, 125, 196, 294, 448};
        return base[n];
    }

    /* Rolling window: a(i-5) … a(i-1) for the current i */
    seq_t a_n_minus5 = 86;    /* a(2)  */
    seq_t a_n_minus4 = 125;   /* a(3)  */
    seq_t a_n_minus3 = 196;   /* a(4)  */
    seq_t a_n_minus2 = 294;   /* a(5)  */
    seq_t a_n_minus1 = 448;   /* a(6)  */

    seq_t a_i;                        /* a(i) to be computed */
    for (idx_t i = 7; i <= n; ++i) {
        a_i = 4 * a_n_minus1
              - 5 * a_n_minus2
              + a_n_minus3
              + 2 * a_n_minus4
              - a_n_minus5;

        /* Shift the window for the next iteration */
        a_n_minus5 = a_n_minus4;
        a_n_minus4 = a_n_minus3;
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = a_i;
    }
    return a_i;   /* a(n) */
}
