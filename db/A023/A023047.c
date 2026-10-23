/*
 * A023047  10th differences of factorial numbers.
 * https://oeis.org/A023047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^10 + 45*n^9 + 915*n^8 + 10950*n^7 + 84693*n^6 +
 *            438963*n^5 + 1531865*n^4 + 3523110*n^3 + 5057331*n^2 + 4036697*n
 *            + 1334961)*n!
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        fact = fact * (seq_t)i;
    }

    /* evaluate P(n) by Horner's method */
    seq_t poly = (seq_t)1;                           /* coefficient of n^10 */
    poly = poly * (seq_t)n + (seq_t)45;
    poly = poly * (seq_t)n + (seq_t)915;
    poly = poly * (seq_t)n + (seq_t)10950;
    poly = poly * (seq_t)n + (seq_t)84693;
    poly = poly * (seq_t)n + (seq_t)438963;
    poly = poly * (seq_t)n + (seq_t)1531865;
    poly = poly * (seq_t)n + (seq_t)3523110;
    poly = poly * (seq_t)n + (seq_t)5057331;
    poly = poly * (seq_t)n + (seq_t)4036697;
    poly = poly * (seq_t)n + (seq_t)1334961;

    return fact * poly;
}
