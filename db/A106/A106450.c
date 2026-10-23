/*
 * A106450  a(n) = A004443(n) if n is odd, a(n) = A004443(n)/2 if n is even.
 * https://oeis.org/A106450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 2
 *   a(n) = n+2          if n ≡ 1 (mod 4)
 *   a(n) = (n-2)/2      if n ≡ 2 (mod 4)
 *   a(n) = n-2          if n ≡ 3 (mod 4)
 *   a(n) = n/2 + 1      if n ≡ 0 (mod 4) and n > 0
 *   (these follow from a(4*k+1)=4*k+3, a(4*k+2)=2*k, a(4*k+3)=4*k+1,
 *    a(4*k+4)=2*k+3)
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t N = (seq_t)n;
    switch (n & 3) {
    case 0:         /* n ≡ 0 mod 4, n>0 */
        return N / 2 + (seq_t)1;
    case 1:         /* n ≡ 1 mod 4 */
        return N + (seq_t)2;
    case 2:         /* n ≡ 2 mod 4 */
        return (N - (seq_t)2) / (seq_t)2;
    case 3:         /* n ≡ 3 mod 4 */
        return N - (seq_t)2;
    }
    /* unreachable */
    return (seq_t)0;
}
