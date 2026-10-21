/*
 * A083416  Add 1, double, add 1, double, etc.
 * https://oeis.org/A083416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; for n>1: a(n)= (n%2==0)? a(n-1)+1 : 2*a(n-1)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined behavior below offset, return 0 */
    seq_t val = 1;                /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        if (i & 1U)          /* odd i: double the previous term */
            val <<= 1;       /* val = 2 * val */
        else                  /* even i: add one */
            val += 1;
    }
    return val;
}
