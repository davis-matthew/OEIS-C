/*
 * A011565  Stirling numbers of second kind S2(16,n).
 * https://oeis.org/A011565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = S(16,n) – the Stirling number of the second kind for a
 *            16‑element set
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence entry for index n (1‑based).  For n > 16 the value is 0. */

seq_t a(idx_t n)
{
    /* A table of S(16,k) for k = 0..16.  The table is built once on first use. */
    static int built = 0;
    static seq_t table[17];

    if (!built)
    {
        /* Compute S(16,k) via the recurrence S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
        seq_t s[17] = {0};
        s[0] = 1;                 /* S(0,0) = 1 */

        for (idx_t i = 1; i <= 16; ++i)
        {
            seq_t next[17] = {0};
            for (idx_t k = 1; k <= i; ++k)
                next[k] = k * s[k] + s[k-1];
            for (idx_t k = 0; k <= i; ++k)
                s[k] = next[k];
        }

        for (idx_t k = 0; k <= 16; ++k)
            table[k] = s[k];

        built = 1;
    }

    if (n <= 16)
        return table[n];

    return (seq_t)0;
}
