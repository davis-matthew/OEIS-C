/*
 * A026218  a(n) = (1/3)*(s(n) + 2), where s(n) is the n-th number congruent
 *          to 1 mod 3 in A026177.
 * https://oeis.org/A026218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(s(n)+2) where s(n) are numbers ≡1 (mod 3) in
 *            A026177 * Using the closed form derived from the pattern: * if n
 *            mod 4 == 1: a(n) = (n + 1) / 2 * if n mod 4 == 2: a(n) = (3*n -
 *            2) / 2 * if n mod 4 == 3: a(n) = (3*n - 1) / 2 * if n mod 4 ==
 *            0: a(n) = 3*n / 2
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; 0 is defined as 0 */
    seq_t sn = (seq_t)n;
    idx_t r  = n & 3ULL;                      /* fast modulo 4 */

    switch (r) {
        case 1:   return (sn + 1) / 2;          /* n ≡ 1 (mod 4) */
        case 2:   return (sn * 3 - 2) / 2;      /* n ≡ 2 (mod 4) */
        case 3:   return (sn * 3 - 1) / 2;      /* n ≡ 3 (mod 4) */
        case 0:   return (sn * 3) / 2;          /* n ≡ 0 (mod 4) */
    }
    return 0;                                   /* unreachable, satisfies compiler */
}
