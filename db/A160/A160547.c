/*
 * A160547  Numbers coprime to 31.
 * https://oeis.org/A160547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-1)/30)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence lists positive integers that are not multiples of 31. */
    /* For n >= 1: a(n) = n + ⌊(n-1)/30⌋.  The result is accurate
       whenever it fits in the type seq_t. */
    if (n == 0) {
        /* Undefined for n == 0 in OEIS offset, but return 0 safely. */
        return (seq_t)0;
    }
    idx_t q = (n - 1) / 30;            /* floor((n-1)/30) */
    seq_t val = (seq_t)n + (seq_t)q;   /* n + q */
    return val;
}
