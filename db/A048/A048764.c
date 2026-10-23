/*
 * A048764  Largest factorial <= n.
 * https://oeis.org/A048764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest factorial <= n
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n<1, but defensive */

    seq_t fac = 1;          /* factorial value */
    idx_t k = 1;            /* current factorial index */

    /* Multiply until adding the next factor would exceed n */
    while (1) {
        /* Compute floor division n/(k+1) in seq_t */
        seq_t div = (seq_t)n / (seq_t)(k + 1);

        /* If fac > div, next factorial would exceed n (or overflow) */
        if (fac > div)
            break;

        /* Safe multiplication: result <= n, fits in seq_t */
        fac = fac * (seq_t)(k + 1);
        k++;
    }
    return fac;
}
