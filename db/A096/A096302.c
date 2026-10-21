/*
 * A096302  Number of combinations of two natural numbers that together have n
 *          digits.
 * https://oeis.org/A096302
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 81*(n-1)*10^(n-2) (offset 2, a(2)=81)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is not defined in OEIS
       and any return value will be ignored.  Returning 0 is safe. */
    if (n < 2) {
        return 0;
    }

    /* Compute 81*(n-1) first. */
    seq_t result = (seq_t)81;
    result *= (n - 1);

    /* Compute 10^(n-2) using binary exponentiation to minimise
       the number of large integer multiplications. */
    seq_t pow = 1;      /* 10^0 */
    seq_t ten = 10;     /* 10^1 */
    idx_t exp = n - 2;  /* exponent (n-2) is >= 0 */

    while (exp > 0) {
        if (exp & 1) {
            pow *= ten;
        }
        ten *= ten;
        exp >>= 1;
    }

    result *= pow;
    return result;
}
