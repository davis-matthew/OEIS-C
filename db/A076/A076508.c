/*
 * A076508  Expansion of 2*x*(1+4*x+8*x^2)/(1-24*x^3).
 * https://oeis.org/A076508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*24^k if n mod 3 = 1 * 8*24^k if n mod 3 = 2 * 16*24^k
 *            if n mod 3 = 0 * where k = ⌊(n‑1)/3⌋ * * The sequence is defined
 *            by the generating function * A(x) = 2*x*(1+4*x+8*x^2) /
 *            (1‑24*x^3). * The coefficient extraction yields the above closed
 *            form
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n=0 is out of range but shall not occur */
        return (seq_t)0;

    /* k = floor((n-1)/3) */
    idx_t k = (n - 1) / 3;

    /* compute 24^k in *pow24* */
    seq_t pow24 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow24 *= (seq_t)24;

    /* base coefficient according to n mod 3 */
    seq_t base;
    switch (n % 3) {
        case 1: base = (seq_t)2;  break;
        case 2: base = (seq_t)8;  break;
        case 0: default: base = (seq_t)16; break;
    }

    return pow24 * base;
}
