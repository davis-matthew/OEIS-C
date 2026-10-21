/*
 * A094327  a(n) = 9 written in base n.
 * https://oeis.org/A094327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9 written in base n (base‑1 gives nine 1's)
 * Range:     u8 none, u16 none, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base‑1 special case: unary expansion of 9 → nine 1's */
    if (n == 1U)
        return 111111111;

    /* for n ≥ 10, 9 < n so representation is the single digit 9 */
    if (n >= 10U)
        return 9;

    /* n in {2,…,9} – compute the base‑n representation of 9 */
    seq_t temp = 9;
    int digits[10];
    int len = 0;
    /* collect digits least significant first */
    while (temp > 0) {
        digits[len++] = (int)(temp % (int)n);
        temp /= (int)n;
    }

    /* build the resulting integer by concatenating digits in correct order */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
