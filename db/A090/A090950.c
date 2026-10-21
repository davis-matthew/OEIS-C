/*
 * A090950  a(n) = (1/24)*(n+1)*(n+3)*(n^2+22*n+88).
 * https://oeis.org/A090950
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(n+1)*(n+3)*(n^2+22*n+88) offset -1
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 560, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A090950.
 * The sequence is defined for n >= -1 with a(-1) = 0.
 * For n >= 0, the explicit formula above is used.
 * Intermediate calculations are performed in 128‑bit integer
 * arithmetic to avoid overflow for all supported seq_t sizes.
 */
seq_t a(idx_t n)
{
    if (n == -1)            /* offset: first term is a(-1) = 0 */
        return (seq_t)0;

    seq_t nn = (seq_t)n;                /* cast index to seq_t type */
    seq_t t1 = nn + (seq_t)1;           /* (n + 1)   */
    seq_t t2 = nn + (seq_t)3;           /* (n + 3)   */
    seq_t t3 = nn * nn + (seq_t)22 * nn + (seq_t)88;  /* n^2 + 22n + 88 */

    /* Use a 128‑bit accumulator for multiplication before division. */
    unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2 *
                            (unsigned __int128)t3;
    prod /= (unsigned __int128)24;      /* divide by 24 exactly */

    return (seq_t)prod;                 /* cast back to seq_t */
}
