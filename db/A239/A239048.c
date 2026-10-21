/*
 * A239048  1 plus the n-th divisor of 24.
 * https://oeis.org/A239048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 1 + A018253(n) (n-th divisor of 24)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Divisors of 24 in increasing order */
    static const seq_t divs[8] = {1, 2, 3, 4, 6, 8, 12, 24};

    /* n is 1‑based; compute (n‑1) mod 8 efficiently */
    idx_t idx = (n - 1) & 7ULL;          /* 8 = 2³ */

    return divs[idx] + (seq_t)1;
}
