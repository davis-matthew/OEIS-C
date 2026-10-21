/*
 * A058365  Number of ways to cover (without overlapping) a ring lattice
 *          (necklace) of n sites with molecules that are 8 sites wide.
 * https://oeis.org/A058365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A058365: Number of ways to cover a ring lattice 
   (necklace) of n sites with 8‑site–wide molecules.
   Recurrence: a(n) = a(n-1) + a(n-8) for n ≥ 9,
   with initial values a(1)=…=a(7)=1, a(8)=9. 
   This implementation evaluates the recurrence iteratively on each call. */

seq_t a(idx_t n)
{
    /* Handle trivial/known base cases directly. */
    if (n == 0)   return (seq_t)0;          /* undefined for n<1, safe fallback */
    if (n <= 7)   return (seq_t)1;
    if (n == 8)   return (seq_t)9;

    /* Ring buffer holding the last eight computed terms:
       vals[0] = a(i-7), vals[1] = a(i-6), ..., vals[7] = a(i)   */
    seq_t vals[8];
    /* Initialise buffer with a(1)..a(8). */
    for (idx_t i = 0; i < 7; ++i) vals[i] = (seq_t)1;
    vals[7] = (seq_t)9;

    /* Compute a(9) .. a(n) iteratively. */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = vals[7] + vals[0];     /* a(i-1) + a(i-8) */

        /* Shift values left by one position. */
        for (int k = 0; k < 7; ++k) vals[k] = vals[k + 1];
        vals[7] = next;                     /* store a(i) */
    }

    return vals[7];                          /* a(n) */
}
