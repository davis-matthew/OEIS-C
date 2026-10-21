/*
 * A098305  Unsigned member r=-5 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A098305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2) - a(n-3), a(0)=0, a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)5;   /* a(n-1) */
    seq_t an = 0;

    /* Compute iteratively using the 3‑term linear recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        an = 6*a2 + 6*a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
