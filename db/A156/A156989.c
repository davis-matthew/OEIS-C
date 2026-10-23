/*
 * A156989  Largest size of a subset of {1,2,3}^n that does not contain any
 *          combinatorial lines (i.e., strings formed by 1, 2, 3, and at least
 *          one instance of a wildcard x, with x then substituted for 1, 2, or
 *          3, e.g. 12x3x gives the combinatorial line 12131, 12232, 12333.)
 * https://oeis.org/A156989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = 50 * 3^(n-4) for n ≥ 5; a(0) = 1, a(1) = 2, a(2) = 6,
 *            a(3) = 18, a(4) = 52
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑computed values for the first five indices */
    static const seq_t base[5] = { (seq_t)1, (seq_t)2, (seq_t)6,
                                  (seq_t)18, (seq_t)52 };

    if (n < 5)
        return base[n];

    /* compute 3^(n-4) using binary exponentiation */
    idx_t exp = n - 4;
    seq_t result = (seq_t)1;
    seq_t base3  = (seq_t)3;

    while (exp != 0)
    {
        if (exp & 1u)
            result *= base3;
        base3 *= base3;
        exp >>= 1;
    }

    /* multiply by 50 to obtain the final value */
    result *= (seq_t)50;
    return result;
}
