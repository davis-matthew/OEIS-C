/*
 * A021620  Decimal expansion of 1/616.
 * https://oeis.org/A021620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digits of 1/616 a(0) = 0, a(1) = 0, a(2) = 1, for
 *            n ≥ 3 a(n) = {6,2,3,3,7,6}[ (n‑3) mod 6 ]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;
    if (n == 2)
        return 1;

    static const seq_t pattern[6] = { 6, 2, 3, 3, 7, 6 };
    return pattern[(n - 3) % 6];
}
