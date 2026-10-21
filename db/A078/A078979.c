/*
 * A078979  a(n) = A078711(n) - 1.
 * https://oeis.org/A078979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A078711(n) - 1. From the OEIS data this reduces to a
 *            simple case analysis: if n ≡ 0 (mod 6) → a(n) = (v₂(n/6)) mod 3
 *            (number of trailing zero bits of n/6 modulo 3) if n ≡ 3 (mod 6)
 *            → a(n) = 2 if n ≡ 2 or 5 (mod 6)→ a(n) = 1 if n ≡ 1 or 4 (mod
 *            6)→ a(n) = 0 where v₂(x) is the exponent of 2 i...
 * Range:     u8 n <= 9330, u16 n <= 9330, u32 n <= 9330, u64 n <= 9330,
 *            u128 n <= 9330, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* remainder of n modulo 6 */
    idx_t r = n % 6;

    /* multiples of 6: compute v₂(n/6) mod 3 */
    if (r == 0) {
        idx_t k = n / 6;          /* k ≥ 1 for n ≥ 6 */
        unsigned tz = 0;           /* trailing‑zero counter */
        while ((k & 1) == 0) {     /* while k is even */
            tz++;
            k >>= 1;
        }
        return (seq_t)(tz % 3);
    }

    /* other residues */
    if (r == 3)                /* 3 mod 6 */
        return (seq_t)2;

    if (r == 2 || r == 5)      /* 2 or 5 mod 6 */
        return (seq_t)1;

    /* r == 1 or r == 4 */
    return (seq_t)0;
}
