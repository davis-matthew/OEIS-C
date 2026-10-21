/*
 * A070424  a(n) = 7^n mod 41.
 * https://oeis.org/A070424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 41
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t base = 7;
    const seq_t mod  = 41;

    /* 7^40 ≡ 1 (mod 41) ⇒ period 40 */
    idx_t exp = n % 40;

    seq_t result = 1 % mod;
    seq_t power  = base % mod;

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * power) % mod;
        power = (power * power) % mod;
        exp >>= 1ULL;
    }
    return result;
}
