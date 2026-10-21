/*
 * A055122  Base-12 complement of n (write n in base 12, then replace each
 *          digit with its base-12 negative).
 * https://oeis.org/A055122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = apply base‑12 complement to each digit of n. * For every
 *            base‑12 digit d (0 ≤ d < 12), the complementary digit is * d' =
 *            0 if d == 0 * d' = 12 – d otherwise * The result is the number
 *            formed by those complemented digits in the * same base 12 order.
 *            This is equivalent to * a(n) = Σ (d' * 12...
 * Range:     u8 n <= 143, u16 n <= 9606, u32 n <= 9606, u64 n <= 9606,
 *            u128 n <= 9606, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;
    seq_t power = 1;

    while (n > 0)
    {
        idx_t digit   = n % 12;
        idx_t newdig  = (digit == 0) ? 0 : 12 - digit;
        res += (seq_t)newdig * power;
        power *= 12;
        n /= 12;
    }

    return res;
}
