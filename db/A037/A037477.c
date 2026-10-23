/*
 * A037477  a(n) = Sum{d(i)*9^i: i=0,1,...,m}, where Sum{d(i)*8^i:
 *          i=0,1,...,m} is the base 8 representation of n.
 * https://oeis.org/A037477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ d_i * 9^i where d_i are the base‑8 digits of n * (i.e.
 *            interpret the base‑8 representation of n as a base‑9 number)
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t      tn    = n;          /* working copy of n */
    seq_t      res   = 0;          /* result accumulator */
    seq_t      pow9  = 1;          /* current power of 9 (9^i) */

    while (tn > 0)                 /* while there are still digits       */
    {
        idx_t digit = tn & 7;      /* n % 8   (digit in base 8)         */
        res += (seq_t)digit * pow9;/* add digit * 9^i                    */
        pow9 *= (seq_t)9;          /* advance to next power of 9        */
        tn >>= 3;                  /* n /= 8   (shift right by 3 bits) */
    }
    return res;
}
