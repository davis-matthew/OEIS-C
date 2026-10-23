/*
 * A021839  Decimal expansion of 1/835.
 * https://oeis.org/A021839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1) / 835)) mod 10 * This returns the n-th
 *            decimal digit after the point in 1/835, * where a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t MOD = (seq_t)835;   /* modulus for the fraction 1/835 */
    seq_t pow10 = (seq_t)1;        /* 10^n mod MOD */
    seq_t base   = (seq_t)10;      /* base for exponentiation */
    idx_t exp = n;

    /* binary exponentiation to compute 10^n mod 835 */
    while (exp > 0) {
        if (exp & 1)
            pow10 = (pow10 * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* digit = (10^(n+1) / 835) mod 10
     * 10^(n+1) mod 835 = (pow10 * 10) mod 835
     * but we only need the integer division by 835, which gives the digit directly. */
    return (pow10 * (seq_t)10) / MOD;
}
