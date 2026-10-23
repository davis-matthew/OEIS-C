/*
 * A040921  Continued fraction for sqrt(952).
 * https://oeis.org/A040921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, mult, easy
 * Formula:   a(0) = 30; For n > 0 let e = the exponent of 2 in the
 *            factorisation of n (i.e. the number of trailing zeros in the
 *            binary representation of n). Then: e = 0 → a(n) = 1 e = 1 → a(n)
 *            = 5 e = 2 → a(n) = 6 e >= 3 → a(n) = 60 (The sequence is
 *            multiplicative and odd‑prime factors contribute 1.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;
    idx_t e = 0;
    idx_t m = n;
    /* Count trailing zeros – exponent of 2 in n */
    while ((m & 1) == 0) { e++; m >>= 1; }
    switch (e)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)6;
        default: return (seq_t)60;
    }
}
