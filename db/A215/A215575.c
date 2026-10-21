/*
 * A215575  a(n) = 7*(a(n-1) - a(n-2) - a(n-3)), with a(0)=3, a(1)=7, a(2)=35.
 * https://oeis.org/A215575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = 7*(a(n-1) - a(n-2) - a(n-3)),  
 * inizial values:  a(0)=3, a(1)=7, a(2)=35
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)35;

    seq_t a0 = (seq_t)3; /* a(n-3) */
    seq_t a1 = (seq_t)7; /* a(n-2) */
    seq_t a2 = (seq_t)35;/* a(n-1) */
    seq_t an = a2;       /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)7 * (a2 - a1 - a0);
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
