/*
 * A094624  Expansion of g.f. x*(1+11*x+x^2)/((1-x)*(1+x)*(1-10*x^2)).
 * https://oeis.org/A094624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 11*a(n-2) - 10*a(n-4) for n > 3 with a(0) = 0, a(1) = 1,
 *            a(2) = 11, a(3) = 12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)12;

    /* Keep only the last four computed values.
       arr[0] = a(i-4), arr[1] = a(i-3), arr[2] = a(i-2), arr[3] = a(i-1) */
    seq_t arr[4];
    arr[0] = (seq_t)0;  /* a(0) */
    arr[1] = (seq_t)1;  /* a(1) */
    arr[2] = (seq_t)11; /* a(2) */
    arr[3] = (seq_t)12; /* a(3) */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t next = (seq_t)11 * arr[2] // a(i-2)
                   - (seq_t)10 * arr[0]; // a(i-4)

        /* Shift the window: drop the oldest and append next */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = next;
    }

    /* When the loop terminates, arr[3] holds a(n) */
    return arr[3];
}
