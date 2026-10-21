/*
 * A092296  a(n) = 3*n + A090193(n).
 * https://oeis.org/A092296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n + ((sum of ternary digits of n + 1) mod 3)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3*n using the target type seq_t to avoid overflow. */
    seq_t three_n = (seq_t)n * 3;

    /* Sum the base‑3 digits of n.  The sum fits easily in an 8‑bit type. */
    idx_t x = n;
    uint8_t sum = 0;
    while (x > 0) {
        sum += (uint8_t)(x % 3);
        x /= 3;
    }

    /* Add the offset (sum + 1) mod 3 to obtain A090193(n). */
    uint8_t offset = (sum + 1) % 3;

    return three_n + offset;
}
