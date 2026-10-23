/*
 * A163617  a(2*n) = 2*a(n), a(2*n + 1) = 2*a(n) + 2 + (-1)^n, for all n in Z.
 * https://oeis.org/A163617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 2*a(n) a(2*n+1) = 2*a(n) + 2 + (-1)^n (n >= 0) => If n
 *            is odd (n = 2*m+1): a(n) = 2*a(m) + 3 if m is even a(n) = 2*a(m)
 *            + 1 if m is odd a(0) = 0
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A163617. */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return 0;

    /* Even indices: a(2*n) = 2*a(n) */
    if ((n & 1) == 0)
        return a(n >> 1) << 1;

    /* Odd indices: a(2*m+1) = 2*a(m) + 3 (if m even) or +1 (if m odd) */
    idx_t m = n >> 1;
    seq_t t = a(m) << 1;
    if ((m & 1) == 0)      /* m even */
        return t + 3;
    else                  /* m odd */
        return t + 1;
}
