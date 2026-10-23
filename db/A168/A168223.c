/*
 * A168223  a(n) = A006369(n) - A006368(n).
 * https://oeis.org/A168223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(12*n) = -10*n, a(12*n+1) = 7*n, a(12*n+2) = -2*n, a(12*n+3) =
 *            -n, a(12*n+4) = -2*n-1, a(12*n+5) = 7*n+3, a(12*n+6) = -10*n-5,
 *            a(12*n+7) = 7*n+4, a(12*n+8) = -2*n-1, a(12*n+9) = -n-1,
 *            a(12*n+10)= -2*n-2, a(12*n+11)= 7*n+7
 * Range:     i8 n <= 155, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Decompose n = 12*k + r, 0 <= r < 12 */
    idx_t r = n % 12;
    seq_t k = (seq_t)(n / 12);

    switch (r) {
        case 0:  return -10 * k;
        case 1:  return  7 * k;
        case 2:  return -2  * k;
        case 3:  return -k;
        case 4:  return -2 * k - 1;
        case 5:  return  7 * k + 3;
        case 6:  return -10 * k - 5;
        case 7:  return  7 * k + 4;
        case 8:  return -2 * k - 1;
        case 9:  return -k - 1;
        case 10: return -2 * k - 2;
        case 11: return  7 * k + 7;
        default: return 0; /* should not happen */
    }
}
