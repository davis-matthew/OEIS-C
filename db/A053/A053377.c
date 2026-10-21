/*
 * A053377  a(n) contains n digits '3' or '8' and is divisible by 2^n.
 * https://oeis.org/A053377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 8, a(n+1) = a(n) + 10^n * (8 - 5 * ((a(n)/2^n) mod 2)), n
 *            >= 1 (equivalently, append an 8 if (a(n)/2^n) is even, otherwise
 *            append a 3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is outside the OEIS offset; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    seq_t val = (seq_t)8;          /* a(1) */
    if (n == 1) return val;

    seq_t pow10 = (seq_t)10;       /* 10^1 for the first appending */

    for (idx_t i = 1; i < n; ++i) {
        /* parity of a(i) / 2^i: 0 = even, 1 = odd           */
        int parity = ((val >> (unsigned int)i) & (seq_t)1) ? 1 : 0;
        seq_t digit = parity ? (seq_t)3 : (seq_t)8;   /* prepend 3 or 8 */

        /* a(i+1) = a(i) + digit * 10^i */
        val = val + digit * pow10;

        /* prepare 10^(i+1) for the next iteration */
        pow10 = pow10 * (seq_t)10;
    }
    return val;
}
