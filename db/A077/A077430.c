/*
 * A077430  a(n) = floor(log_10(2*n^2)) + 1.
 * https://oeis.org/A077430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_10(2*n^2)) + 1
 * Range:     u8 n <= 11, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)         /* guard against an out-of‑range call – undefined behaviour otherwise */
        return 0;

    /* Compute 2*n*n in the result type to avoid any loss of precision on the type used for n. */
    seq_t val = (seq_t)2 * (seq_t)n * (seq_t)n;

    /* Count decimal digits of val. */
    seq_t d = 0;
    while (val != 0)
    {
        val /= (seq_t)10;
        ++d;
    }

    return d;
}
