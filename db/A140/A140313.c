/*
 * A140313  First differences of A140298.
 * https://oeis.org/A140313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-5) + 3*a(n-4) + 3*a(n-3) - a(n-2) - a(n-1) with
 *            initial terms a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 63, u64 n <= 126,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 1;

    /* Circular buffer of the last five terms.
       buf[i] contains a(current_index - 5 + i)   */
    seq_t buf[5] = {1, 0, 0, 1, 1};

    for (idx_t k = 5; k <= n; ++k)
    {
        /* Compute a(k) */
        seq_t val = 3 * buf[(k - 5) % 5];
        val += 3 * buf[(k - 4) % 5];
        val += 3 * buf[(k - 3) % 5];
        val -= buf[(k - 2) % 5] + buf[(k - 1) % 5];

        /* Store the new value in the buffer */
        buf[k % 5] = val;
    }

    return buf[n % 5];
}
