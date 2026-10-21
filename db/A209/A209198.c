/*
 * A209198  a(n) = 1 if n not == 2 (mod 4) and relatively prime to 63
 *          otherwise 0.
 * https://oeis.org/A209198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if the 2‑adic valuation v2(n) ≠ 1 and the 3‑adic
 *            valuation v3(n) < 2 and the 7‑adic valuation v7(n) = 0;
 *            otherwise a(n) = 0. (This reproduces the multiplicative
 *            definition a(2^e)=1⇔e≠1, a(3^e)=1⇔e<2, a(7^e)=0^e, a(p^e)=1
 *            otherwise.)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t x = n;
    unsigned e2 = 0, e3 = 0, e7 = 0;

    /* count factors of 2 */
    while ((x & 1) == 0) {
        e2++;
        x >>= 1;          /* same as x /= 2; faster on unsigned */
    }

    /* count factors of 3 */
    while (x % 3 == 0) {
        e3++;
        x /= 3;
    }

    /* count factors of 7 */
    while (x % 7 == 0) {
        e7++;
        x /= 7;
    }

    /* the rest of the prime factors are irrelevant (value 1) */
    if (e2 == 1 || e3 >= 2 || e7 >= 1)
        return (seq_t)0;
    else
        return (seq_t)1;
}
