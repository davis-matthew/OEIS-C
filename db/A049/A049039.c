/*
 * A049039  Geometric Connell sequence: 1 odd, 2 even, 4 odd, 8 even, ...
 * https://oeis.org/A049039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, tabf
 * Formula:   a(n) = 2*n - 1 - floor(log_2(n)) (offset 1)
 * Range:     u8 n <= 131, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (The build guarantees idx_t is unsigned 64‑bit) */
    if (n == 0)                 /* safety for index 0 – not used by OEIS */
        return 0;

    /* Compute floor(log2(n)) by shifting right until zero. */
    idx_t log2n = 0;
    idx_t t = n >> 1;           /* skip the lowest bit */
    while (t) {
        log2n++;
        t >>= 1;
    }

    /* Compute 2*n - 1 - log2n using the wider index type. */
    idx_t result = (2 * n) - 1 - log2n;

    return (seq_t)result;       /* cast to the output type */
}
