/*
 * A037463  a(n)=Sum{d(i)*9^i: i=0,1,...,m}, where Sum{d(i)*4^i: i=0,1,...,m}
 *          is the base 4 representation of n.
 * https://oeis.org/A037463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{d(i)*9^i : i = 0,1,...,m}, where d(i) are the digits
 *            of n in base‑4
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function interprets the base‑4 representation of n as a base‑9 number. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow9   = 1;
    idx_t tmp    = n;
    while (tmp > 0) {
        seq_t digit = (seq_t)(tmp & 3ULL);  /* tmp % 4 */
        result += digit * pow9;
        pow9 *= (seq_t)9;
        tmp >>= 2;                         /* tmp /= 4 */
    }
    return result;
}
