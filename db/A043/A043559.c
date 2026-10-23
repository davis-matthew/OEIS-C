/*
 * A043559  Number of runs in base-7 representation of n.
 * https://oeis.org/A043559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in the base-7 representation of n (a run
 *            is a maximal block of equal consecutive digits)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* base-7 representation is "0" */
        return (seq_t)1;

    seq_t runs = (seq_t)1;   /* at least one run */
    int prevDigit = -1;      /* no previous digit yet */

    idx_t tmp = n;
    while (tmp > 0) {
        int digit = (int)(tmp % (idx_t)7);  /* least‑significant digit */
        tmp /= (idx_t)7;
        if (prevDigit != -1 && digit != prevDigit)
            runs++;     /* a new run starts when digit changes */
        prevDigit = digit;
    }

    return runs;
}
