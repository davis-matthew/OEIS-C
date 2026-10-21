/*
 * A055123  Base-13 complement of n (write n in base 13, then replace each
 *          digit with its base-13 negative).
 * https://oeis.org/A055123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ ((13 - d_i) mod 13) * 13^i, where d_i are the base‑13
 *            digits of n (replacing 0 with 0, and 1…12 with 12…1)
 * Range:     u8 n <= 168, u16 n <= 2196, u32 n <= 2196, u64 n <= 2196,
 *            u128 n <= 2196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t power = 1;
    idx_t temp = n;
    while (temp > 0) {
        unsigned digit = (unsigned)(temp % 13);
        seq_t complement = (digit == 0) ? 0 : (13 - digit);
        res += complement * power;
        power *= 13;
        temp /= 13;
    }
    return res;
}
