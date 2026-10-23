/*
 * A151670  G.f.: Product_{k>=0} (1 + 2*x^(5^k)).
 * https://oeis.org/A151670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(number of 1 digits in base‑5 of n) if all base‑5
 *            digits of n are < 2; otherwise a(n) = 0 (OEIS A151670)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    int ones = 0;                /* count of 1‑digits in base 5 */

    /* Scan the base‑5 representation */
    while (temp > 0) {
        idx_t digit = temp % 5;  /* digit in base 5 */
        if (digit > 1)           /* invalid digit -> coefficient 0 */
            return 0;
        ones += (int)digit;      /* digit is 0 or 1 */
        temp /= 5;
    }

    /* Compute 2^ones using only seq_t arithmetic */
    seq_t result = 1;
    for (int i = 0; i < ones; ++i)
        result *= 2;

    return result;
}
