/*
 * A055804  a(n) = T(n,n-4), array T as in A055801.
 * https://oeis.org/A055804
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n^4 -28*n^3 +178*n^2 -416*n +441) + * (-1)^n*(4*n^3
 *            -90*n^2 + 704*n -1977))/768, with a(4) = 1
 * Range:     u8 n <= 21, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)          /* defined only for n ≥ 4; behaviour below offset is irrelevant */
        return 0;

    if (n == 4)         /* special case */
        return 1;

    __int128 nn = n;    /* intermediate 128‑bit arithmetic */

    __int128 base = 2*nn*nn*nn*nn
                   - 28*nn*nn*nn
                   + 178*nn*nn
                   - 416*nn
                   + 441;

    __int128 add = 4*nn*nn*nn
                   - 90*nn*nn
                   + 704*nn
                   - 1977;

    /* multiply add by (-1)^n */
    if (n & 1)          /* odd n */
        add = -add;

    __int128 val = base + add;

    return (seq_t)(val / 768);   /* exact division; numerator divisible by 768 */
}
