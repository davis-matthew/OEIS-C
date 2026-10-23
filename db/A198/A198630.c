/*
 * A198630  Alternating sums of powers of 1,2,...,7.
 * https://oeis.org/A198630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..7} (-1)^(j+1) * j^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= 7; ++j) {
        seq_t pw = pow_seq_t((seq_t)j, n);
        if (j & 1U)     /* odd j : positive sign */
            sum += pw;
        else            /* even j : negative sign */
            sum -= pw;
    }
    return sum;
}
