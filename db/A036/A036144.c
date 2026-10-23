/*
 * A036144  a(n) = 2^n mod 131.
 * https://oeis.org/A036144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 131
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Simple binary exponentiation modulo 131. */
    const seq_t MOD = (seq_t)131;
    seq_t result = 1;
    seq_t base   = 2;
    idx_t exp = n;

    while (exp) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}
