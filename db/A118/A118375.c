/*
 * A118375  Minimum over all permutations b of 1..n of sum b(i)*b^{-1}(i).
 * https://oeis.org/A118375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)/6 + ((n%4==2||n%4==3)?1:0)
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Split the product to avoid intermediate overflow on small seq_t types. */
    seq_t x = nn;
    seq_t y = nn + 1;
    seq_t z = nn + 2;

    /* Divide by 2 */
    if (x % 2 == 0)
        x /= 2;
    else if (y % 2 == 0)
        y /= 2;
    else
        z /= 2;

    /* Divide by 3 */
    if (x % 3 == 0)
        x /= 3;
    else if (y % 3 == 0)
        y /= 3;
    else
        z /= 3;

    seq_t t = x * y * z;

    /* Add e(n) = 1 if n ≡ 2,3 mod 4, else 0 */
    idx_t r = n & 3;  /* n % 4 */
    if (r == 2 || r == 3)
        t += 1;

    return t;
}
