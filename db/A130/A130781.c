/*
 * A130781  Sequence is identical to its third differences: a(n+3) = 3*a(n+2)
 *          - 3*a(n+1) + 2*a(n), with a(0)=a(1)=1, a(2)=2.
 * https://oeis.org/A130781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) with a(0)=1, a(1)=1,
 *            a(2)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)2;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        unsigned __int128 t = ((unsigned __int128)a2 * 3) -
                             ((unsigned __int128)a1 * 3) +
                             ((unsigned __int128)a0 * 2);
        next = (seq_t)t;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
