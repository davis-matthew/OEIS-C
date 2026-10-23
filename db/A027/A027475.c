/*
 * A027475  a(n) = (n-1) * 15^(n-2).
 * https://oeis.org/A027475
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * 15^(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                      /* values below the offset are undefined */

    /* Compute 15^(n-2) using exponentiation by squaring */
    idx_t exp = n - 2;
    seq_t result = 1;
    seq_t base   = 15;

    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return ((seq_t)(n - 1)) * result;
}
