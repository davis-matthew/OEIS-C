/*
 * A021389  Decimal expansion of 1/385.
 * https://oeis.org/A021389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 0 for n < 2; a(n) = pat[(n-2) mod 6] for n ≥ 2; where pat
 *            = {2, 5, 9, 7, 4, 0}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first two digits are 0. */
    if (n < 2) {
        return 0;
    }

    /* Pattern of digits after the initial zeros. */
    static const seq_t pat[6] = {2, 5, 9, 7, 4, 0};

    /* Zero‑based index into the repeating block. */
    idx_t idx = (n - 2) % 6;

    return pat[idx];
}
