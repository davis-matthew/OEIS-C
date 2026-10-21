/*
 * A092196  Number of letters in "old style" Roman numeral representation of n
 *          (e.g., IIII rather than IV).
 * https://oeis.org/A092196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ g(d_i) where d_i are the decimal digits of n * and g(d)
 *            = d if d < 5 * = d - 4 otherwise (i.e., d = 5..9)
 * Range:     u8 n <= 3999, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0)
    {
        int d = n % 10;     /* extract least‑significant decimal digit */
        if (d < 5)
            sum += d;       /* 0,1,2,3,4  → 0,1,2,3,4 letters */
        else
            sum += d - 4;   /* 5,6,7,8,9  → 1,2,3,4,5 letters */
        n /= 10;
    }
    return sum;
}
