/*
 * A024655  n written in fractional base 9/7.
 * https://oeis.org/A024655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 179, u64 n <= 2384,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): representation of integer n in fractional base 9/7.
   The algorithm repeatedly extracts the least‑significant digit d
   as d = n mod 9, appends it to the decimal representation, then
   replaces n by (n - d)/9 * 7 (carry 7 when a digit exceeds 8).
   This yields the sequence listed in A024655. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    const seq_t A = (seq_t)9;   /* base numerator */
    const seq_t B = (seq_t)7;   /* base denominator (carry) */

    seq_t value = 0;   /* resulting decimal number */
    seq_t mult  = 1;   /* current power of 10 for placing digits */
    seq_t v     = n;   /* working copy of the input */

    while (v > 0)
    {
        seq_t digit = v % A;               /* least significant digit */
        value += digit * mult;            /* prepend digit to result */
        mult *= 10;

        /* apply carry: replace v by (v - digit)/A * B */
        v = ((v - digit) / A) * B;
    }

    return value;
}
