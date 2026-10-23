/*
 * A141495  a(n) = 3*a(n-1) for n>2; a(0)=1, a(1)=3, a(2)=7.
 * https://oeis.org/A141495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 7; a(n) = 3 * a(n-1) for n > 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases. */
    if (n == 0u) {
        return (seq_t)1;
    }
    if (n == 1u) {
        return (seq_t)3;
    }
    if (n == 2u) {
        return (seq_t)7;
    }

    /* For n > 2 compute 7 * 3^(n-2) by repeated multiplication. */
    seq_t result = (seq_t)7;
    idx_t steps = n - 2u;  /* n > 2, so this is non‑negative. */
    for (idx_t i = 0u; i < steps; ++i) {
        result *= (seq_t)3;
    }
    return result;
}
