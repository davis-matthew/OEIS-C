/*
 * A028744  Nonsquares mod 31.
 * https://oeis.org/A028744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 31 * floor((n-1)/15) + b[(n-1) mod 15] where b =
 *            {3,6,11,12,13,15,17,21,22,23,24,26,27,29,30}
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth positive integer that is a non‑quadratic residue modulo 31. */
seq_t a(idx_t n)
{
    /* Precomputed list of non-square residues mod 31, sorted ascending. */
    static const seq_t nonsq[15] = {
        3, 6, 11, 12, 13,
        15, 17, 21, 22, 23,
        24, 26, 27, 29, 30
    };

    /* Adjust indexing: n >= 1. */
    idx_t zero_based = n - 1;
    idx_t cycle_index = zero_based % 15;      /* in [0,14] */
    idx_t cycles = zero_based / 15;           /* number of complete cycles */

    /* Compute result: 31 * cycles + nonsq[cycle_index] */
    seq_t term = (seq_t)31 * (seq_t)cycles + nonsq[cycle_index];
    return term;
}
