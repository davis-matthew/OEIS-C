/*
 * A168211  a(n) = (9 + 14*n + 23*(-1)^n)/4.
 * https://oeis.org/A168211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9 + 14*n + 23*(-1)^n)/4
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the parity‑based simplified formulas:
       if n is odd      -> a = 7 * ((n-1)/2)
       if n is even     -> a = (7*n + 16) / 2
       All operations are performed on seq_t after casting the index.
    */
    if (n & (idx_t)1) {
        /* odd n */
        return (seq_t)7 * ((seq_t)(n - 1) / 2);
    } else {
        /* even n */
        return ((seq_t)7 * (seq_t)n + (seq_t)16) / 2;
    }
}
