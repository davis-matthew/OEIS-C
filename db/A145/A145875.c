/*
 * A145875  Repdigit Kaprekar numbers.
 * https://oeis.org/A145875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = d(n) * R(floor((3*n+2)/5)) where R(k) = (10^k - 1)/9 is a
 *            repunit of length k and d(n) follows the 15‑periodic cycle
 *            {1,9,5,9,9,7,9,2,9,9,4,9,8,9,9}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; behaviour below is irrelevant */

    /* compute the length of the repunit: k = floor((3*n + 2)/5) */
    idx_t k = (3 * n + 2) / 5;

    /* 10^k as seq_t (iterate multiply by 10) */
    seq_t pow10k = 1;
    for (idx_t i = 0; i < k; ++i)
        pow10k *= 10;

    /* R(k) = (10^k - 1)/9 */
    seq_t repunit = (pow10k - 1) / 9;

    /* d(n) cycle */
    static const seq_t d_cycle[15] = {1, 9, 5, 9, 9,
                                      7, 9, 2, 9, 9,
                                      4, 9, 8, 9, 9};

    seq_t d = d_cycle[(n - 1) % 15];

    return d * repunit;
}
