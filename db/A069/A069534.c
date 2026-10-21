/*
 * A069534  Smallest multiple of 5 with digit sum n.
 * https://oeis.org/A069534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10*n for 1 ≤ n ≤ 4 a(n) = (((n+4)%9)+1) *
 *            10^floor((n+4)/9) - 5 for n > 4
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
        return (seq_t)10 * (seq_t)n;

    /* Compute the remainder of (n+4) modulo 9 */
    idx_t rem = (n + (idx_t)4) % (idx_t)9;
    seq_t first = (seq_t)rem + (seq_t)1;          /* value 1..9 */

    /* Compute the exponent floor((n+4)/9) */
    idx_t exp = (n + (idx_t)4) / (idx_t)9;

    /* Compute 10^exp in seq_t, avoiding overflow in intermediate types */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow10 *= (seq_t)10;

    /* Assemble the final result */
    seq_t result = first * pow10;
    return result - (seq_t)5;
}
