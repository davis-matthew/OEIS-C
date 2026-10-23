/*
 * A121832  Expansion of 1/(1-x-x^5-x^6).
 * https://oeis.org/A121832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-5) + a(n-6), a(0) = 1, a(1) = 1, a(2) = 1,
 *            a(3) = 1, a(4) = 1, a(5) = 2, a(6) = 4
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 65, u64 n <= 128,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed sized buffer to hold the last 7 computed terms. */
    seq_t buf[7];

    /* Base values (offset 0).  All constants are converted to seq_t. */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)1;
    buf[3] = (seq_t)1;
    buf[4] = (seq_t)1;
    buf[5] = (seq_t)2;
    buf[6] = (seq_t)4;

    /* If the requested index is in the base set, return it directly. */
    if (n <= 6u) {
        return buf[(idx_t)n];
    }

    /* Compute terms from 7 up to n, only keeping 7 values in the buffer. */
    for (idx_t i = 7u; i <= n; ++i) {
        seq_t next = buf[(i - 1u) % 7u]
                    + buf[(i - 5u) % 7u]
                    + buf[(i - 6u) % 7u];
        buf[i % 7u] = next;
    }

    /* The requested term is now stored at position n % 7. */
    return buf[n % 7u];
}
