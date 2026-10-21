/*
 * A208778  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 1 vertically.
 * https://oeis.org/A208778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) for n > 4
   Initial values: a(1) = 26, a(2) = 676, a(3) = 156, a(4) = 260 */

seq_t a(idx_t n)
{
    /* Handle indexes below the offset (optional) */
    if (n == 0)
        return 0;

    if (n == 1)
        return 26;
    if (n == 2)
        return 676;
    if (n == 3)
        return 156;
    if (n == 4)
        return 260;

    seq_t a_prev = 156; /* a(3) */
    seq_t a_curr = 260; /* a(4) */
    idx_t i;

    /* Compute a(5) .. a(n) iteratively */
    for (i = 5; i <= n; ++i) {
        seq_t a_next = a_prev + a_curr;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
