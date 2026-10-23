/*
 * A171640  a(n) = 10*a(n-1)-a(n-2)-4 with a(1)=1 and a(2)=3.
 * https://oeis.org/A171640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) - 4, a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed iteratively; no memoisation is needed for n≤10000. */

seq_t a(idx_t n)
{
    /* Undefined for n==0 – return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Initial values. */
    seq_t a1 = (seq_t)1;   /* a(1) */
    if (n == 1) {
        return a1;
    }
    seq_t a2 = (seq_t)3;   /* a(2) */
    if (n == 2) {
        return a2;
    }

    /* Iteratively compute up to a(n). */
    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)10 * a2 - a1 - (seq_t)4;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
