/*
 * A192849  Molecular topological indices of the triangular graphs.
 * https://oeis.org/A192849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - 1)*(n-2)^2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 85, u64 n <= 7132,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the triangular graph molecular topological indices.
   The function is defined for all n >= 1, following OEIS offset 1.
   For n < 3 the value is 0, consistent with the formula. */
seq_t a(idx_t n)
{
    if (n < 3)                      /* a(1) = a(2) = 0 */
        return (seq_t)0;

    seq_t nn = (seq_t)n;            /* cast index to seq_t for safety */

    /* Use 128‑bit intermediate arithmetic when seq_t is 64‑bit or less
       to avoid overflow in the intermediate multiplications.  For larger
       seq_t types (e.g., 128‑bit or arbitrary‑precision) the same code
       works because the cast to unsigned __int128 can be promoted to the
       required type. */
    unsigned __int128 tn   = (unsigned __int128)nn;
    unsigned __int128 tn2  = tn * tn;                  /* n^2          */
    unsigned __int128 diff = tn - 2;                   /* n-2          */
    unsigned __int128 diff2 = diff * diff;             /* (n-2)^2      */

    unsigned __int128 part = tn * (tn2 - 1);           /* n*(n^2-1)    */
    unsigned __int128 result = part * diff2;           /* final product */

    return (seq_t)result;
}
