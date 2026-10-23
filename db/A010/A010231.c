/*
 * A010231  Continued fraction for sqrt(189).
 * https://oeis.org/A010231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=13; for n>=1: a(n)=1 if n is odd, 2 if n≡2 (mod 4), 26 if
 *            n≡0 (mod 4). Equivalent multiplicative definition: a(2)=2,
 *            a(2^e)=26 for e≥2, a(p^e)=1 for odd prime p
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)13;
    switch (n % 4) {
    case 1: return (seq_t)1;
    case 2: return (seq_t)2;
    case 3: return (seq_t)1;
    case 0: return (seq_t)26;
    }
    return (seq_t)0; /* unreachable */
}
