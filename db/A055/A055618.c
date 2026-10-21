/*
 * A055618  a(n) = A*10^(4*n-1) + B with A = 990*(10^(4*n)-1)/9999 and B =
 *          990000*(10^(4*n-4)-1)/9999 + 100.
 * https://oeis.org/A055618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 990*(10^(4*n)-1)/9999 * 10^(4*n-1) +
 *            (990000*(10^(4*n-4)-1)/9999 + 100)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Compute 10^(4*n-4) once */
    seq_t pow4minus4 = pow10(4*n - 4);          /* 10^(4*n-4)  */
    seq_t pow4n = pow4minus4 * ((seq_t)10000); /* 10^(4*n)    */

    /* A = 990*(pow4n - 1)/9999 */
    seq_t A = ((seq_t)990 * (pow4n - (seq_t)1)) / ((seq_t)9999);

    /* B = 990000*(pow4minus4 - 1)/9999 + 100 */
    seq_t B = ((seq_t)990000 * (pow4minus4 - (seq_t)1)) / ((seq_t)9999);
    B += (seq_t)100;

    /* 10^(4*n-1) = pow4n / 10 */
    seq_t pow10_4n_minus1 = pow4n / ((seq_t)10);

    return A * pow10_4n_minus1 + B;
}
