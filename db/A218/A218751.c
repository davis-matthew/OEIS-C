/*
 * A218751  a(n) = (48^n - 1)/47.
 * https://oeis.org/A218751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*a(n-1) + 1 with a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0U) {
        return 0;
    }

    seq_t val = 0;
    /* Iterate n times, each time computing the recurrence
       a(k) = 48*a(k-1) + 1.  The multiplication by 48 is done
       directly on the arbitrary‑precision type seq_t. */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)48 + 1;
    }
    return val;
}
