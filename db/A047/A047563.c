/*
 * A047563  Numbers that are congruent to {0, 3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + [0,3,4,5,6,7][ (n-1) mod 6 ]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047563: Numbers ≡ {0,3,4,5,6,7} (mod 8).  Index starts at 1. */
seq_t a(idx_t n)
{
    /* Indexes are 1‑based, but we work with 0‑based numbers internally. */
    idx_t idx0 = n - 1;                 /* 0‑based index of the term */
    idx_t block = idx0 / 6;             /* number of complete 6‑length blocks before this term */
    idx_t pos   = idx0 % 6;             /* position inside the current block (0‑based) */

    /* Mapping of positions 0…5 to the allowed residues in each 8‑number block. */
    static const seq_t MAP[6] = {0, 3, 4, 5, 6, 7};

    /* Compute the value: base = 8 * block ;  then add the residue. */
    seq_t res = (seq_t)block * (seq_t)8 + MAP[pos];
    return res;
}
