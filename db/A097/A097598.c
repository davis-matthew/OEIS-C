/*
 * A097598  Describe n from right to left.
 * https://oeis.org/A097598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of <count><digit> pairs for each run of
 *            equal decimal digits in the representation of n, processed from
 *            the least‑significant digit to the most‑significant one
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is a special case – its only digit is 0, occurring once. */
    if (n == 0)
        return 10;   /* 1 (count) followed by 0 (digit) */

    seq_t result = 0;
    idx_t temp = n;

    /* Process runs of identical digits starting from the least‑significant digit. */
    while (temp > 0) {
        int digit = temp % 10;   /* current digit */
        idx_t cnt = 0;

        /* Count how many times the current digit repeats consecutively. */
        while (temp > 0 && temp % 10 == digit) {
            cnt++;
            temp /= 10;
        }

        /* Append the pair <cnt><digit> to the result. */
        result = result * 100 + (seq_t)(cnt * 10 + digit);
    }

    return result;
}
