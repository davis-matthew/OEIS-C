/*
 * A231963  Concatenate n with its UPC check digit, a(n) = 10*n + A237042(n).
 * https://oeis.org/A231963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*n + c(n), where c(n) is the UPC check digit * computed
 *            from the decimal digits of n as follows: * Let the digits of n
 *            be d[0] (units), d[1], d[2], … from right to left. * Compute S =
 *            sum_{i even} 3*d[i] + sum_{i odd} d[i]. * The check digit is c =
 *            (10 - (S mod 10)) mod 10. * This matche...
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* There is no defined term for n = 0, but guard against it. */
        return 0;
    }

    /* Compute weighted sum of digits from least significant digit. */
    seq_t sum = 0;
    idx_t pos = 0;
    idx_t temp = n;
    while (temp > 0) {
        idx_t digit = temp % 10;          /* digit is 0..9 */
        if ((pos & 1) == 0) {             /* even position from right (0,2,4,...) */
            sum += (seq_t)digit * 3;
        } else {
            sum += (seq_t)digit;
        }
        pos++;
        temp /= 10;
    }

    /* Determine the check digit: c = (10 - (sum mod 10)) mod 10. */
    idx_t mod = (idx_t)(sum % 10);          /* sum % 10 is 0..9 */
    seq_t check = (seq_t)(10 - mod) % 10;

    /* Return 10*n + check. */
    return (seq_t)n * 10 + check;
}
