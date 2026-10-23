/*
 * A107732  Column 1 of the array in A107735.
 * https://oeis.org/A107732
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n/2-1) for n even (n ≥ 4), a(n) = (2^(n-1) - 1) / 3
 *            for n odd (n ≥ 3) The sequence is defined for n ≥ 3; values for
 *            n < 3 are undefined and this implementation simply returns 0
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two(idx_t exp)
{
    seq_t result = 1;   /* 2^0 */
    seq_t base   = 2;   /* 2^1 */
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3)          /* behaviour below the offset is unused */
        return 0;

    if ((n & 1U) == 0)      /* even n  →  a(n) = 2^(n/2-1) */
    {
        idx_t k = (n >> 1) - 1U;   /* n/2 - 1  */
        return power_of_two(k);
    }
    else                    /* odd n   →  a(n) = (2^(n-1)-1)/3 */
    {
        seq_t v = power_of_two(n - 1U);
        v -= 1;
        return v / 3;
    }
}
