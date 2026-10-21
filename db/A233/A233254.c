/*
 * A233254  Number of n X 6 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Formula: a(n) = 1664*a(n-1) - 471040*a(n-2) + 44826624*a(n-3) - 1358954496*a(n-4) */
/*  Initial values: a(1) = 136, a(2) = 166400, a(3) = 220626944, a(4) = 294517735424 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)136ULL;
    if (n == 2) return (seq_t)166400ULL;
    if (n == 3) return (seq_t)220626944ULL;
    if (n == 4) return (seq_t)294517735424ULL;

    /* Sliding window for the recurrence: a1 = a(n-4), a2 = a(n-3),
       a3 = a(n-2), a4 = a(n-1).  Initialize with the known first four terms. */
    seq_t a1 = (seq_t)136ULL;           /* a(1)   */
    seq_t a2 = (seq_t)166400ULL;        /* a(2)   */
    seq_t a3 = (seq_t)220626944ULL;     /* a(3)   */
    seq_t a4 = (seq_t)294517735424ULL;  /* a(4)   */

    /* Compute terms from a(5) up to a(n) using the recurrence. */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t temp = (seq_t)1664 * a4;
        temp -= (seq_t)471040 * a3;
        temp += (seq_t)44826624 * a2;
        temp -= (seq_t)1358954496 * a1;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = temp;
    }

    return a4;  /* a(n) */
}
