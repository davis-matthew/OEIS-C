/*
 * A216212  Number of n step walks (each step +-1 starting from 0) which are
 *          never more than 4 or less than -4.
 * https://oeis.org/A216212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 5*a(n-2) - 5*a(n-4) (n ≥ 5) Initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8, a(4)=16
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)16;

    /* For n ≥ 5 compute iteratively using the recurrence */
    seq_t arr[4] = {(seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16};
    /* arr[0] = a(i-4), arr[1] = a(i-3), arr[2] = a(i-2), arr[3] = a(i-1) */

    idx_t i = 5;
    for (; i <= n; ++i) {
        seq_t cur = (seq_t)5 * arr[2] - (seq_t)5 * arr[0];
        /* Shift the window */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = cur;
    }
    return arr[3]; /* a(n) */
}
