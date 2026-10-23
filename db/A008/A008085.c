/*
 * A008085  Coordination sequence T2 for Zeolite Code EDI.
 * https://oeis.org/A008085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008085: Coordination sequence T2 for Zeolite Code EDI
 * Formula: a(n) = 288*floor(n/12)^2 + 48*(n%12)*floor(n/12) + C[n%12],
 * where C[0..11] = {0,4,12,18,32,52,74,100,128,162,204,244}.
 * Special cases: a(0) = 1, a(2) = 8.  For k = 0 (i.e. n multiple of 12),
 * the constant term is -2, which is handled by subtracting 2 from the base.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;       /* a(0) = 1 */
    if (n == 2) return 8;       /* a(2) = 8 */

    idx_t m = n / 12;           /* floor division */
    idx_t k = n % 12;           /* remainder */

    /* Base polynomial part */
    seq_t result = (seq_t)288 * (seq_t)m * (seq_t)m
                 + (seq_t)48 * (seq_t)k * (seq_t)m;

    static const seq_t C[12] = {
        0, 4, 12, 18, 32, 52, 74, 100, 128, 162, 204, 244
    };

    if (k == 0)
    {
        /* For multiples of 12: subtract 2 to get the correct offset */
        result -= 2;
    }
    else
    {
        result += C[k];
    }

    return result;
}
