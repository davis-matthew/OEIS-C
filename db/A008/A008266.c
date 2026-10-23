/*
 * A008266  Coordination sequence T1 for Zeolite Code GIS.
 * https://oeis.org/A008266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(12*m + k) = 264*m^2 + 44*k*m + C[k], with C = [2, 4, 9, 18,
 *            32, 48, 67, 92, 120, 150, 185, 224], and for n=0, a(0) = 1
 * Range:     u8 n <= 11, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)1;

    /* Constants for each k = 0..11 */
    static const unsigned int C[12] = {
        2, 4, 9, 18, 32, 48,
        67, 92, 120, 150, 185, 224
    };

    /* Compute m and k */
    idx_t m = n / 12;
    idx_t k = n % 12;

    /* Compute the formula using seq_t intermediate values */
    seq_t sm = (seq_t)m;
    seq_t sk = (seq_t)k;

    seq_t val = 264ULL * sm * sm;           /* 264*m^2   */
    val += 44ULL * sm * sk;                 /* 44*k*m    */
    val += (seq_t)C[k];                     /* C[k]      */

    return val;
}
