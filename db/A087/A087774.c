/*
 * A087774  a[1] = 1, a[2] = a[3] = 2; a[n] = 3*a[a[n-1]] -
 *          3*a[n-a[n-1]]+a[n-3].
 * https://oeis.org/A087774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087774: a(0)=11, a(n)=1 if n≡0 (mod 3),
 * a(n)=2 if n≡1 (mod 3), a(n)=n if n≡2 (mod 3). */
seq_t a(idx_t n)
{
    /* Special case for the offset 0 value */
    if (n == 0)
        return (seq_t)11;

    /* n modulo 3 determines the value */
    switch (n % 3)
    {
        case 0: return (seq_t)1;   /* n ≡ 0 (mod 3) */
        case 1: return (seq_t)2;   /* n ≡ 1 (mod 3) */
        case 2: return (seq_t)n;   /* n ≡ 2 (mod 3) */
        default: return (seq_t)0;   /* unreachable */
    }
}
