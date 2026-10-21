/*
 * A200782  Expansion of 1 / (1 - 6*x + 20*x^3 - 15*x^4 + x^6).
 * https://oeis.org/A200782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 20*a(n-3) + 15*a(n-4) - a(n-6) Initial values:
 *            a(0) = 1, a(1) = 6, a(2) = 36, a(3) = 196, a(4) = 1071, a(5) =
 *            5796
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)196;
    if (n == 4) return (seq_t)1071;
    if (n == 5) return (seq_t)5796;

    /* Array for the last six computed terms:
       arr[0] = a(n-6), arr[1] = a(n-5), ..., arr[5] = a(n-1)   */
    seq_t arr[6];

    /* Initialize with the known first six values */
    arr[0] = (seq_t)1;      /* a(0)   */
    arr[1] = (seq_t)6;      /* a(1)   */
    arr[2] = (seq_t)36;     /* a(2)   */
    arr[3] = (seq_t)196;    /* a(3)   */
    arr[4] = (seq_t)1071;   /* a(4)   */
    arr[5] = (seq_t)5796;   /* a(5)   */

    /* Iteratively compute a(6) through a(n) */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t new_val = 6 * arr[5]            /* 6*a(n-1) */
                      - 20 * arr[3]           /* 20*a(n-3) */
                      + 15 * arr[2]           /* 15*a(n-4) */
                      - arr[0];              /* a(n-6)   */

        /* Shift the ring buffer: drop oldest, advance, insert new */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = arr[5];
        arr[5] = new_val;
    }

    /* After the loop, arr[5] holds a(n) */
    return arr[5];
}
