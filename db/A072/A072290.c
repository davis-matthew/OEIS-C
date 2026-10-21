/*
 * A072290  Number of digits in the decimal expansion of the Champernowne
 *          constant that must be scanned to encounter all n-digit strings.
 * https://oeis.org/A072290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + n*10^n - (10^n - 10)/9 (for n ≥ 1), a(0) = 1. All
 *            intermediate values are computed with the arithmetic operations
 *            supported for type seq_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0. */
    if (n == 0)
        return 1;

    /* Compute 10^n using binary exponentiation. */
    seq_t base = 10;  /* 10^1 */
    seq_t result = 1; /* 10^0 */
    idx_t exp = n;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)           /* square only if more bits remain */
            base *= base;
    }
    seq_t pow10 = result;          /* 10^n  */

    /* Compute (10^n - 10)/9.  Since pow10 ≥ 10 for n ≥ 1,
       this subtraction is safe for unsigned seq_t. */
    seq_t part2 = (pow10 - 10) / 9;

    /* Convert n to seq_t for the other multiplications. */
    seq_t ns = (seq_t)n;

    /* Final formula: a(n) = n + n*10^n - part2. */
    seq_t prod = ns * pow10;      /* n * 10^n */
    seq_t result_a = ns + prod - part2;
    return result_a;
}
