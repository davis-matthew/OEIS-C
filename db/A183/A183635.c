/*
 * A183635  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4)
 * Initial values:
 *   a(1) = 136
 *   a(2) = 340
 *   a(3) = 952
 *   a(4) = 2884
 *
 * This implementation computes a(n) using the above linear recurrence.
 * It works for any n ≥ 1, uses only integer arithmetic, and is
 * independent of any build‑specific utilities.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for offset 1, but safe guard */

    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)136;
    if (n == 2) return (seq_t)340;
    if (n == 3) return (seq_t)952;
    if (n == 4) return (seq_t)2884;

    /* Iterate the recurrence from i = 5 up to n. */
    seq_t a1 = 136, a2 = 340, a3 = 952, a4 = 2884, an;
    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)10 * a4 - (seq_t)35 * a3 + (seq_t)50 * a2 - (seq_t)24 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
