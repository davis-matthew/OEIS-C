/*
 * A240775  The six values n in each interval [i*840, (i+1)*840), where i >=
 *          0, for which Mordell's formulas do NOT provide a
 *          three-Egyptian-fraction solution for 4/n.
 * https://oeis.org/A240775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 840 * floor((n-1)/6) + {1,121,169,289,361,529}[(n-1)%6]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The six residues are the n values in each block [i*840,(i+1)*840) for which
   Mordell's formulas do NOT provide a 3‑Egyptian‑fraction solution of 4/n. */

seq_t a(idx_t n)
{
    static const seq_t residues[6] = {
        (seq_t)1,
        (seq_t)121,
        (seq_t)169,
        (seq_t)289,
        (seq_t)361,
        (seq_t)529
    };

    /* Compute the 0‑based block number and position within the block. */
    idx_t block = (n - (idx_t)1) / (idx_t)6;
    unsigned pos  = (unsigned)((n - (idx_t)1) % (idx_t)6);

    /* Compute the result using seq_t arithmetic.  The multiplication
       block*840 is performed in seq_t to avoid intermediate overflow
       for the 64‑bit build; for smaller builds the value truncates
       correctly where it is needed. */
    seq_t value = (seq_t)block * (seq_t)840 + residues[pos];
    return value;
}
