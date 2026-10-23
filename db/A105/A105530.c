/*
 * A105530  Ternary modular Gray code for n.
 * https://oeis.org/A105530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ternary modular Gray code (OEIS A105530).
   Convert n to base‑3 digits d_k…d_0.  
   The Gray digits g_k…g_0 are produced as follows:
        g_k = d_k
        for i = k-1 .. 0:
            if d_i > d_{i+1}  then g_i = d_i - d_{i+1}
            if d_i == d_{i+1} then g_i = 0
            if d_i <  d_{i+1} then g_i = d_i + 3 - d_{i+1}
   The integer a(n) is the base‑3 number formed by g_k…g_0.          */

seq_t a(idx_t n)
{
    /* Store ternary digits, least significant first. 64 is enough
       for all n up to 10 000 (3^9 > 10 000). */
    idx_t digits[64];
    idx_t len = 0;
    idx_t tmp = n;

    /* Handle n == 0 specially so that len >= 1 */
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 3;
            tmp /= 3;
        }
    }

    /* The most significant digit is digits[len-1].
       Its Gray value is the same.  Start the result with it. */
    seq_t result = (seq_t)digits[len-1];

    /* Process the remaining digits from second‑most significant
       down to the least significant, using the *original* previous
       digit as the comparison key. */
    for (idx_t i = len-1; i > 0; --i) {
        idx_t b = digits[i-1];        /* current original digit   */
        idx_t a_digit = digits[i];    /* previous original digit  */
        seq_t g;

        if (b > a_digit)
            g = (seq_t)(b - a_digit);
        else if (b == a_digit)
            g = (seq_t)0;
        else /* b < a_digit */
            g = (seq_t)(b + 3 - a_digit);

        result = result * 3 + g;
    }

    return result;
}
