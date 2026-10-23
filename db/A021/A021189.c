/*
 * A021189  Decimal expansion of 1/185.
 * https://oeis.org/A021189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (10^(n+1) mod 1850) / 185 This computes the n‑th digit of
 *            the decimal expansion of 1/185
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)1850;   /* 185 * 10 */
    const seq_t DIFF = (seq_t)185;   /* divisor */

    /* exponent = n + 1 */
    idx_t exp = n + 1;

    seq_t result = 1 % MOD;
    seq_t base   = 10 % MOD;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    return result / DIFF;
}
