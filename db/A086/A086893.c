/*
 * A086893  a(n) is the index of F(n+1) at the unique occurrence of the
 *          ordered pair of reversed consecutive terms (F(n+1),F(n)) in
 *          Stern's diatomic sequence A002487, where F(k) denotes the k-th
 *          term of the Fibonacci sequence A000045.
 * https://oeis.org/A086893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(n+2)=4*a(n)+1 for n≥1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe return */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t val;
    if ((n & 1) != 0) {          /* n is odd: use odd subsequence */
        val = (seq_t)1;          /* a(1) */
        for (idx_t i = 3; i <= n; i += 2)
            val = (seq_t)4 * val + (seq_t)1;   /* a(i)=4*a(i-2)+1 */
    } else {                     /* n is even: use even subsequence */
        val = (seq_t)3;          /* a(2) */
        for (idx_t i = 4; i <= n; i += 2)
            val = (seq_t)4 * val + (seq_t)1;   /* a(i)=4*a(i-2)+1 */
    }
    return val;
}
