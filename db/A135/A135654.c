/*
 * A135654  Divisors of 8128 (the 4th perfect number), written in base 2.
 * https://oeis.org/A135654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, easy, less
 * Formula:   a(n) = A133024(n), written in base 2. For n ≤ 7: a(n) = 1
 *            followed by (n‑1) zeros (decimal). For 8 ≤ n ≤ 14: a(n) = 7 ones
 *            followed by (n‑8) zeros. For n > 14: value is undefined; returns
 *            0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* safety for out‑of‑range values */
        return 0;

    /* n ≤ 7 : one followed by (n-1) zeros */
    if (n <= 7)
    {
        seq_t val = 1;         /* start with the single "1" digit */
        for (idx_t i = 1; i < n; ++i) /* multiply by 10 (add a "0") */
            val *= 10;
        return val;
    }

    /* 8 ≤ n ≤ 14 : seven "1" digits followed by (n-8) zeros */
    if (n <= 14)
    {
        seq_t val = 1111111;    /* decimal value of seven ones */
        idx_t zeros = n - 8;    /* number of trailing zeros to add */
        for (idx_t i = 0; i < zeros; ++i)
            val *= 10;
        return val;
    }

    /* Undefined beyond the 14th divisor */
    return 0;
}
