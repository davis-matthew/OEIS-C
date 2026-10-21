/*
 * A083177  Let P(k) = floor(k/2). Start with n, apply P repeatedly until
 *          reach 1. a(n) = concatenation of numbers obtained.
 * https://oeis.org/A083177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenation of the differences between successive
 * terms in the sequence n, floor(n/2), floor(n/4), …, 1,
 * followed by an extra terminating 1.
 * For example: n = 11 → 11,5,2,1 → differences: 6,3,1 → a(11)=6311.
 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t cur = n;

    while (cur > 1) {
        idx_t next = cur >> 1;                     /* floor(cur/2) */
        seq_t diff = (seq_t)cur - (seq_t)next;     /* difference */

        /* Count decimal digits of diff */
        int digits = 0;
        seq_t temp = diff;
        while (temp > 0) {
            temp /= (seq_t)10;
            ++digits;
        }

        /* Shift result by the number of digits and append diff */
        for (int i = 0; i < digits; ++i) {
            result *= (seq_t)10;
        }
        result += diff;

        cur = next;
    }

    /* Append the final terminating 1 */
    result *= (seq_t)10;
    result += (seq_t)1;

    return result;
}
