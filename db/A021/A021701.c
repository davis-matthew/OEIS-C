/*
 * A021701  Decimal expansion of 1/697.
 * https://oeis.org/A021701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) / 697) % 10 (0‑indexed digits of 1/697)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^n mod 697  (stored in seq_t) */
    const seq_t mod = (seq_t)697;
    seq_t result = (seq_t)1;   /* 10^0 mod 697 */
    seq_t base = (seq_t)10;    /* 10 mod 697 */

    idx_t exp = n;
    /* binary exponentiation */
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    /* result now equals 10^n mod 697; compute the digit */
    seq_t tmp = result * (seq_t)10;
    return tmp / (seq_t)697;
}
