/*
 * A160652  Express n in balanced ternary, then reverse the digits, leaving
 *          any trailing zeros alone.
 * https://oeis.org/A160652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, look, sign
 * Formula:   a(n) is obtained by taking the balanced ternary representation
 *            of n, removing the least‑significant zero digits (trailing
 *            zeros), reversing the remaining digits, and interpreting the
 *            resulting balanced ternary number. This implements OEIS A160652
 * Range:     i8 n <= 121, i16 n <= 6561, i32 n <= 6561, i64 n <= 6561,
 *            i128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* balanced ternary digits of n, least‑significant first */
    int digits[64];
    int k = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        int rem = tmp % 3;
        tmp /= 3;
        if (rem == 0) {
            digits[k++] = 0;
        } else if (rem == 1) {
            digits[k++] = 1;
        } else { /* rem == 2 */
            digits[k++] = -1;
            tmp += 1;               /* carry */
        }
    }

    /* count trailing zeros (least‑significant zeros in the balanced ternary
       representation) */
    int t = 0;
    while (t < k && digits[t] == 0) ++t;
    int m = k - t;                     /* number of digits to reverse */

    seq_t result = 0;
    seq_t power = 1;
    /* power = 3^t, the weight of the first reversed digit */
    for (int i = 0; i < t; ++i) power *= 3;

    for (int j = 0; j < m; ++j) {
        int rev_digit = digits[k - 1 - j];
        result += (seq_t)rev_digit * power;
        power *= 3;
    }
    return result;
}
