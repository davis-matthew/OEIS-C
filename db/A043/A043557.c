/*
 * A043557  Number of runs in base-5 representation of n.
 * https://oeis.org/A043557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043557: number of runs in base‑5 representation of n */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* "0" has one run */

    seq_t runs = 1;                     /* at least one run */
    idx_t m = n;
    int prev = m % 5;                   /* least significant digit */
    m /= 5;

    while (m > 0)                       /* process remaining digits */
    {
        int cur = m % 5;
        if (cur != prev) {
            runs++;                     /* new run starts */
            prev = cur;
        }
        m /= 5;
    }
    return runs;
}
