/*
 * A180713  If n is even then a(n) = 3n, if n == 1 mod 4 then a(n) = 3n+1, if
 *          n == 3 mod 4 then a(n) = 3n+2.
 * https://oeis.org/A180713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n if n is even * a(n) = 3n + 1 if n ≡ 1 (mod 4) * a(n) =
 *            3n + 2 if n ≡ 3 (mod 4)
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = ((seq_t)n) * 3;          /* 3*n for all n */
    if (n & 1) {                        /* odd n */
        if ((n & 3) == 1)               /* n ≡ 1 (mod 4) */
            ans += 1;                   /* 3n + 1 */
        else                            /* n ≡ 3 (mod 4) */
            ans += 2;                   /* 3n + 2 */
    }
    return ans;
}
