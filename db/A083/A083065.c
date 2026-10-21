/*
 * A083065  4th row of number array A083064.
 * https://oeis.org/A083065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*5^n + 1) / 4 * Recurrence: a(0) = 1 , a(n) = 5*a(n-1)
 *            - 1 (n > 0) * The recurrence avoids intermediate values that
 *            overflow a 64‑bit word. * It works for any seq_t: for
 *            arbitrary‑precision builds it stays exact; * for 64‑bit builds
 *            it stays exact for every n whose term fits in uint64_t
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;          /* a(0) */
    idx_t  i;

    for (i = 1; i <= n; ++i)
        res = (seq_t)5 * res - (seq_t)1;

    return res;
}
