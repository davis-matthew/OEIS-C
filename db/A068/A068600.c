/*
 * A068600  Number of n-uniform tilings having n different arrangements of
 *          polygons about their vertices.
 * https://oeis.org/A068600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A068600 is known to have non‑zero values only for
   n = 1 … 7.  All n-uniform tilings with n distinct vertex types
   are classified in the literature; for n ≥ 8 no such tiling exists.
   Therefore a(n) = 11,20,39,33,15,10,7 for n=1…7 and a(n)=0 for n≥8. */

seq_t a(idx_t n)
{
    /* Return the pre‑computed values for n = 1 … 7, otherwise 0. */
    switch (n) {
        case 1: return (seq_t)11;
        case 2: return (seq_t)20;
        case 3: return (seq_t)39;
        case 4: return (seq_t)33;
        case 5: return (seq_t)15;
        case 6: return (seq_t)10;
        case 7: return (seq_t)7;
        default: return (seq_t)0;
    }
}
