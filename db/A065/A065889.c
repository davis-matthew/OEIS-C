/*
 * A065889  a(n) = number of unicyclic connected simple graphs whose cycle has
 *          length 4.
 * https://oeis.org/A065889
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)(n-2)(n-3)*n^(n-4)/2 for n >= 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;           /* undefined for n < 4, return 0 */

    /* Compute the polynomial factor (n-1)(n-2)(n-3)/2 */
    seq_t factor = (seq_t)(n-1);
    factor *= (n-2);
    factor *= (n-3);
    factor /= 2;

    /* Compute n^(n-4) */
    seq_t power = pow_seq((seq_t)n, n-4);

    /* Final result */
    return factor * power;
}
