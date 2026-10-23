/*
 * A010403  Squares mod 42.
 * https://oeis.org/A010403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth distinct quadratic residue modulo 42, sorted in
 *            increasing order (i.e., the set of all r with r ≡ k² (mod 42))
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 16 distinct residues of squares modulo 42 in ascending order */
    static const seq_t residues[16] = {
        (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)7,
        (seq_t)9, (seq_t)15, (seq_t)16, (seq_t)18,
        (seq_t)21, (seq_t)22, (seq_t)25, (seq_t)28,
        (seq_t)30, (seq_t)36, (seq_t)37, (seq_t)39
    };

    if (n < (idx_t)1 || n > (idx_t)16)
        return (seq_t)0;          /* out‑of‑range values are undefined, return 0 safely */

    return residues[n - 1];        /* n is 1‑based in OEIS */
}
