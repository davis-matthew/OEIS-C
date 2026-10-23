/*
 * A100401  Digital root of 3^n.
 * https://oeis.org/A100401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 3^n mod 18
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digital root of 3^n: 1 for n=0, 3 for n=1, 9 for n>=2. */
seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)3;
    return (seq_t)9;
}
