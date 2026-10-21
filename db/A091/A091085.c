/*
 * A091085  a(n) = mod(A078008(n),10).
 * https://oeis.org/A091085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0: a(n)=6 if n%4==0, 0 if n%4==1, 2 if n%4==2 or 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the periodic last‑digit pattern of A078008 */

/* a(n) = A078008(n) mod 10 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* n%4 can be obtained by masking bits, efficient for uint64_t */
    switch (n & 3u)          /* n modulo 4 */
    {
        case 0:  return (seq_t)6;  /* n ≡ 0 (mod 4), n>0 */
        case 1:  return (seq_t)0;  /* n ≡ 1 (mod 4) */
        case 2:  return (seq_t)2;  /* n ≡ 2 (mod 4) */
        case 3:  return (seq_t)2;  /* n ≡ 3 (mod 4) */
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
