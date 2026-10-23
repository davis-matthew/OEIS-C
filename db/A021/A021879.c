/*
 * A021879  Decimal expansion of 1/875.
 * https://oeis.org/A021879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit n of 1/875 after the decimal point,
 *            0‑based. The expansion is 0.001142857142857..., so the first
 *            three digits are 0,0,1, followed by the 6‑digit repeating cycle
 *            1,4,2,8,5,7
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first three non‑repeating digits */
    const seq_t nonrep[3] = {0, 0, 1};
    /* 6‑digit repeating cycle after the first three digits */
    const seq_t cycle[6] = {1, 4, 2, 8, 5, 7};

    if (n < 3)
        return nonrep[n];
    else
        return cycle[(n - 3) % 6];
}
