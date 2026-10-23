/*
 * A100088  Expansion of (1-x^2)/((1-2*x)*(1+x^2)).
 * https://oeis.org/A100088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + { 0, if n%4=1; -2, if n%4=2; 0, if n%4=3; +2,
 *            if n%4=0 } , a(0)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses simple O(n) iteration, works for any unsigned idx_t range. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t prev = 1;          /* a(0) */
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t cur = prev * 2;          /* 2*a(i-1) */
        if (i % 4 == 0) {
            cur += 2;                  /* add 2 for pattern index 0 (n%4==0) */
        } else if (i % 4 == 2) {
            cur -= 2;                  /* subtract 2 for pattern index 2 (n%4==2) */
        }
        prev = cur;
    }
    return prev;
}
