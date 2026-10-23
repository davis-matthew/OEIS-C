/*
 * A047250  Numbers that are congruent to {0, 3, 4, 5} (mod 6).
 * https://oeis.org/A047250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n - 3 + (-1)^n - 2*(-1)^{n(n-1)/2}) / 4 Simplified by
 *            observing the parity of n and n mod 4: let base = 6*n; switch (n
 *            mod 4) 0 -> a = (base - 4) / 4 1 -> a = (base - 6) / 4 2 -> a =
 *            base / 4 3 -> a = (base - 2) / 4
 * Range:     u8 n <= 42, u16 n <= 9600, u32 n <= 9600, u64 n <= 9600,
 *            u128 n <= 9600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1, but formula works for any n>=1 */
    seq_t base = (seq_t)n * (seq_t)6;

    /* use the lower two bits of n to determine n mod 4 */
    switch (n & 3UL) {
    case 0UL:
        return (base - (seq_t)4) / (seq_t)4;
    case 1UL:
        return (base - (seq_t)6) / (seq_t)4;
    case 2UL:
        return base / (seq_t)4;
    case 3UL:
        return (base - (seq_t)2) / (seq_t)4;
    /* `n & 3` can only be 0..3, but a return is required by
       the compiler to satisfy all control paths. */
    default:
        return (seq_t)0;
    }
}
