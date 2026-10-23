/*
 * A160169  Lodumo_4 of Lucas numbers .
 * https://oeis.org/A160169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*(k-1) + r, where r = Lucas(n) mod 4 and k is the count
 *            of Lucas numbers <= Lucas(n) that are congruent to r modulo 4.
 *            This gives a permutation of the nonnegative integers
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count of previously seen Lucas numbers for each residue modulo 4. */
    idx_t count[4] = {0, 0, 0, 0};

    /* Lucas numbers modulo 4.  L0 = 2, L1 = 1. */
    idx_t lprev2 = 2 % 4;  /* L0 mod 4 */
    idx_t lprev1 = 1 % 4;  /* L1 mod 4 */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t r;          /* residue of the current Lucas number modulo 4 */
        if (i == 0)
            r = lprev2;
        else if (i == 1)
            r = lprev1;
        else
        {
            /* Compute the next Lucas number modulo 4. */
            idx_t lcurr = (lprev1 + lprev2) % 4;
            r = lcurr;
            lprev2 = lprev1;
            lprev1 = lcurr;
        }

        /* Increment the count for this residue. */
        count[r]++;

        /* If we've reached the desired index, compute the result. */
        if (i == n)
        {
            seq_t result = (seq_t)(count[r] - 1) * 4 + r;
            return result;
        }
    }

    /* The loop always returns; this line is only to satisfy the compiler. */
    return 0;
}
