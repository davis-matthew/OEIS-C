/*
 * A141425  Period 6: repeat [1, 2, 4, 5, 7, 8].
 * https://oeis.org/A141425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-6) for n > 6 a(1)=1, a(2)=2, a(3)=4, a(4)=5, a(5)=7,
 *            a(6)=8
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Index is 1‑based; compute zero‑based remainder */
    idx_t r = (n - 1) % 6;

    /* Cycle pattern */
    static const seq_t pattern[6] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)5,
        (seq_t)7,
        (seq_t)8
    };
    return pattern[r];
}
