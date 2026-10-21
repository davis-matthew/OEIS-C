/*
 * A221266  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some king-move neighbor, with no occupancy greater
 *          than 2.
 * https://oeis.org/A221266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 19*a(n-2) + 8*a(n-3) (for n ≥ 4) Initial
 *            values: a(1)=3, a(2)=19, a(3)=141
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* unused, as offset is 1 */
    if (n == 1) return 3;
    if (n == 2) return 19;
    if (n == 3) return 141;

    seq_t a1 = 3;   /* a(n-3) */
    seq_t a2 = 19;  /* a(n-2) */
    seq_t a3 = 141; /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = 10 * a3;      /* 10 * a(n-1) */
        next += 8 * a1;      /* + 8 * a(n-3) */
        next -= 19 * a2;     /* - 19 * a(n-2)  */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
