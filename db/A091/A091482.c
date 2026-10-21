/*
 * A091482  a(n) = (3*n)^n.
 * https://oeis.org/A091482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base value 3*n.  Use seq_t to allow arbitrary‑precision. */
    seq_t base = (seq_t)(3 * (seq_t)n);
    seq_t result = 1;
    idx_t e = n;

    /* Binary exponentiation: compute base^e, where e = n. */
    while (e)
    {
        if (e & 1) result *= base;
        e >>= 1;
        if (e) base *= base;
    }

    return result;
}
