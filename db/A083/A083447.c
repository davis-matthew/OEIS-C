/*
 * A083447  a(n) = floor( n*R(n)/(n+R(n))), where R(n) is the digit reversal
 *          of n (A004086).
 * https://oeis.org/A083447
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(n * R(n) / (n + R(n))) where R(n) is the decimal
 *            digit reversal of n
 * Range:     u8 n <= 12, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverse the decimal digits of an idx_t (uint64_t) number. */
static inline seq_t reverse_digits(idx_t n)
{
    idx_t rev = 0;
    while (n != 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return (seq_t)rev;
}

seq_t a(idx_t n)
{
    /* For n < 2 the offset is irrelevant; behavior is undefined. */
    if (n < 2) {
        return 0;
    }

    seq_t rev = reverse_digits(n);
    seq_t num  = (seq_t)n * rev;          /* n * R(n)   */
    seq_t den  = (seq_t)n + rev;          /* n + R(n)   */
    return num / den;                    /* floor division */
}
