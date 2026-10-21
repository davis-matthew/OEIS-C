/*
 * A056979  Number of blocks of {1, 0, 1} in binary expansion of n.
 * https://oeis.org/A056979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2n)=a(n), a(2n+1)=a(n)+[n≡2 (mod 4)] */

/* Number of blocks of 101 in the binary expansion of n (offset 1). */
seq_t a(idx_t n)
{
    /* Base case: a(1)=0.  The recurrence works for all n>1. */
    if (n == 1)
        return 0;

    if ((n & 1) == 0)          /* n even : a(2n) = a(n) */
        return a(n >> 1);
    else                        /* n odd : a(2n+1) = a(n) + [n≡2 (mod 4)] */
    {
        idx_t m = n >> 1;      /* floor(n/2) == (2n+1)/2 */
        seq_t res = a(m);
        if ((m & 3) == 2)      /* check m ≡ 2 (mod 4) */
            res += 1;
        return res;
    }
}
