/*
 * A232917  Number of n X 5 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 45*a(n-2) + 15*a(n-3) + 36*a(n-4) - 19*a(n-5)
 *            + 2*a(n-6) (for n ≥ 7). Initial values: a(1)=48, a(2)=396,
 *            a(3)=3612, a(4)=33702, a(5)=316800, a(6)=2986152 Computed
 *            iteratively; works for any order of calls
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Initial values */
    const seq_t a1_init = 48;
    const seq_t a2_init = 396;
    const seq_t a3_init = 3612;
    const seq_t a4_init = 33702;
    const seq_t a5_init = 316800;
    const seq_t a6_init = 2986152;

    if (n <= 6) {
        switch (n) {
            case 1: return a1_init;
            case 2: return a2_init;
            case 3: return a3_init;
            case 4: return a4_init;
            case 5: return a5_init;
            case 6: return a6_init;
        }
    }

    /* Iterate from 7 up to n, sliding a window of the last six terms */
    seq_t a1 = a1_init;
    seq_t a2 = a2_init;
    seq_t a3 = a3_init;
    seq_t a4 = a4_init;
    seq_t a5 = a5_init;
    seq_t a6 = a6_init;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 14 * a6 - 45 * a5 + 15 * a4 + 36 * a3 - 19 * a2 + 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
