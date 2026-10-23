/*
 * A028774  Nonsquares mod 61.
 * https://oeis.org/A028774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028774: Nonsquares mod 61. 
   The sequence lists the natural numbers that are
   not quadratic residues modulo 61. */

seq_t a(idx_t n)
{
    /* Non‑residues modulo 61 (sorted ascending). */
    static const seq_t nonres[30] = {
        (seq_t)2, (seq_t)6, (seq_t)7, (seq_t)8, (seq_t)10,
        (seq_t)11, (seq_t)17, (seq_t)18, (seq_t)21, (seq_t)23,
        (seq_t)24, (seq_t)26, (seq_t)28, (seq_t)29, (seq_t)30,
        (seq_t)31, (seq_t)32, (seq_t)33, (seq_t)35, (seq_t)37,
        (seq_t)38, (seq_t)40, (seq_t)43, (seq_t)44, (seq_t)50,
        (seq_t)51, (seq_t)53, (seq_t)54, (seq_t)55, (seq_t)59
    };

    /* Convert to 0‑based index. */
    idx_t n_minus_1 = n - (idx_t)1;

    /* Determine which full period of 61 the term lies in. */
    idx_t period  = n_minus_1 / (idx_t)30;
    idx_t offset  = n_minus_1 % (idx_t)30;
    idx_t base    = period * (idx_t)61;

    return (seq_t)(base + nonres[offset]);
}
