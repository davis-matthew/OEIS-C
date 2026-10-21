/*
 * A239341  Decimal expansion of 7 + 2021/3003.
 * https://oeis.org/A239341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239341: decimal expansion of 7 + 2021/3003
   a(1) = 7
   a(n) = {6,7,2,9,9,3}[ (n-2) mod 6 ],  n ≥ 2
*/
seq_t a(idx_t n)
{
    if (n == 1u)
        return 7u;
    static const seq_t digits[6] = {6u, 7u, 2u, 9u, 9u, 3u};
    idx_t idx = (n - 2u) % 6u;
    return digits[idx];
}
