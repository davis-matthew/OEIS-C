/*
 * A185454  Trajectory of 5 under repeated application of the map in A185452.
 * https://oeis.org/A185454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 5; for n > 1 : a(n) = a(n‑1)/2 if a(n‑1) is even a(n) =
 *            (5*a(n‑1)+1)/2 if a(n‑1) is odd This recurrence generates the
 *            periodic sequence of A185454
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (n == 1) {
        return (seq_t)5;
    }

    seq_t x = (seq_t)5;          /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        if ((x & (seq_t)1) == 0) {   /* even */
            x = x >> 1;              /* a(n+1) = a(n)/2 */
        } else {                     /* odd */
            x = (x * (seq_t)5 + (seq_t)1) >> 1;  /* (5*x+1)/2 */
        }
    }
    return x;
}
