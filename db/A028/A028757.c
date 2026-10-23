/*
 * A028757  Nonsquares mod 44.
 * https://oeis.org/A028757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 44*floor((n-1)/32) + r[((n-1) mod 32)], where r[k] are
 *            the 32 residues (0‑based index) that are **not** quadratic
 *            residues modulo 44, sorted in ascending order
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table of non-square residues modulo 44. */
static const seq_t _nonsq44[32] = {
    (seq_t)2,  (seq_t)3,  (seq_t)6,  (seq_t)7,
    (seq_t)8,  (seq_t)10, (seq_t)11, (seq_t)13,
    (seq_t)14, (seq_t)15, (seq_t)17, (seq_t)18,
    (seq_t)19, (seq_t)21, (seq_t)22, (seq_t)23,
    (seq_t)24, (seq_t)26, (seq_t)27, (seq_t)28,
    (seq_t)29, (seq_t)30, (seq_t)31, (seq_t)32,
    (seq_t)34, (seq_t)35, (seq_t)38, (seq_t)39,
    (seq_t)40, (seq_t)41, (seq_t)42, (seq_t)43
};

seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the sequence definition. */
    idx_t idx0 = n - 1;                /* zero‑based rank of the term   */
    idx_t block = idx0 / 32;           /* complete groups of 32 residues */
    idx_t pos   = idx0 % 32;           /* position inside the current group */
    /* Build the final value using 64‑bit arithmetic to avoid overflow. */
    seq_t base = (seq_t)44 * (seq_t)block;
    return base + _nonsq44[pos];
}
