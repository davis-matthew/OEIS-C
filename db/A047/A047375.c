/*
 * A047375  Numbers that are congruent to {0, 4, 5} mod 7.
 * https://oeis.org/A047375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers that are congruent to {0, 4, 5} modulo 7. For n ≥ 1: •
 *            if n ≡ 0 (mod 3) → a(n) = 7·(n/3) – 2 • if n ≡ 1 (mod 3) → a(n)
 *            = 7·((n/3)+1) – 7 (3k−2 case) • if n ≡ 2 (mod 3) → a(n) =
 *            7·((n/3)+1) – 3 (3k−1 case) This follows from the pattern a(3k)
 *            = 7k−2, a(3k−1) = 7k−3, a(3k−2) = 7k−7
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset 1. n = 0 is undefined in OEIS, return 0 for safety. */
    if (n == 0)
        return 0;

    /* Integer division gives floor(n/3).  The k needed for the
       3k−1 and 3k−2 cases is floor(n/3)+1, avoiding overflow of n+1/2. */
    idx_t q = n / 3;          /* floor(n/3) */
    seq_t k = (seq_t)q;       /* use seq_t for further multiplication */

    switch (n % 3) {
    case 0: /* n = 3k, a = 7k − 2 */
        return 7 * k - 2;

    case 1: /* n = 3k−2, k = q+1, a = 7k − 7 */
        k = k + 1;
        return 7 * k - 7;

    case 2: /* n = 3k−1, k = q+1, a = 7k − 3 */
        k = k + 1;
        return 7 * k - 3;
    }

    /* Unreachable, but keep compiler happy. */
    return 0;
}
