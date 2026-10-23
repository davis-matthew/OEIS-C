/*
 * A028048  Expansion of 1/((1-3x)(1-4x)(1-9x)(1-11x)).
 * https://oeis.org/A028048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (15*11^(n+3) - 28*9^(n+3) + 48*4^(n+3) - 35*3^(n+3)) /
 *            1680 Equivalent recurrence with initial values: a(0) = 1 a(1) =
 *            27 a(2) = 478 a(3) = 7062 For n >= 4: a(n) = 27*a(n-1) -
 *            251*a(n-2) + 933*a(n-3) - 1188*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 27;
    if (n == 2u) return 478;
    if (n == 3u) return 7062;

    seq_t a0 = 1, a1 = 27, a2 = 478, a3 = 7062;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a4 = 27 * a3;
        a4 -= 251 * a2;
        a4 += 933 * a1;
        a4 -= 1188 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;
}
