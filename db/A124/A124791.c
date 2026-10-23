/*
 * A124791  Row sums of number triangle A124790.
 * https://oeis.org/A124791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n) + (-n+2)*a(n-1) + (-5*n+7)*a(n-2) + 3*(-n+2)*a(n-3) =
 *            0 Recurrence in solved form: a(n) = ((n-2)*a(n-1) +
 *            (5*n-7)*a(n-2) + 3*(n-2)*a(n-3)) / (n+1) Initial values: a(0) =
 *            1 a(1) = 1 a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 1;

    /* a(n-3), a(n-2), a(n-1) for the current loop iteration. */
    seq_t a_nm3 = 1; /* a(0) */
    seq_t a_nm2 = 1; /* a(1) */
    seq_t a_nm1 = 1; /* a(2) */
    seq_t a_n = 0;   /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute the numerator of the recurrence. */
        seq_t term1 = ((seq_t)(i - 2)) * a_nm1;
        seq_t term2 = ((seq_t)(5 * i - 7)) * a_nm2;
        seq_t term3 = ((seq_t)(3 * (i - 2))) * a_nm3;
        seq_t num = term1 + term2 + term3;

        /* Compute a(i) by dividing by (i+1). */
        a_n = num / ((seq_t)(i + 1));

        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
