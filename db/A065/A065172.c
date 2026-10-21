/*
 * A065172  Inverse permutation to A065171.
 * https://oeis.org/A065172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k+1)=4k+1, a(4k+2)=4k+3, a(4k+4)=2k+2 Equivalently: if n is
 *            odd -> a(n)=2*n-1 if n≡2 (mod 4) -> a(n)=n+1 if n≡0 (mod 4) ->
 *            a(n)=n/2 (All indices start at 1.)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1; behaviour for n<1 is irrelevant */
        return 0;

    if (n & 1)             /* odd index */
        return (seq_t)2 * n - 1;

    if ((n & 3) == 2)      /* n ≡ 2 (mod 4) */
        return (seq_t)n + 1;

    return (seq_t)n >> 1;  /* n ≡ 0 (mod 4) : divide by 2 */
}
