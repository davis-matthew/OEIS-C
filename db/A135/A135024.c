/*
 * A135024  Number of atoms in n-th amino acid ordered by the alphabetic order
 *          of their one-letter abbreviations.
 * https://oeis.org/A135024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 9 + side_chain_atoms[n-1], where side_chain_atoms is the
 *            number of atoms in the side chain of the n‑th amino acid
 *            (alphabetically sorted by the standard one‑letter code)
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset */
    idx_t idx = n - 1;             /* convert to 0‑based index */
    if (idx >= 20) return 0;       /* only 20 standard amino acids */

    /* side_chain_atoms for the 20 standard amino acids in
       alphabetical order: A, C, D, E, F, G, H, I, K, L,
       M, N, P, Q, R, S, T, V, W, Y */
    static const seq_t side_chain_atoms[20] = {
        4,  /* A (alanine)  */
        5,  /* C (cysteine) */
        7,  /* D (aspartic acid) */
        10, /* E (glutamic acid) */
        14, /* F (phenylalanine) */
        1,  /* G (glycine) */
        11, /* H (histidine) */
        13, /* I (isoleucine) */
        15, /* K (lysine) */
        13, /* L (leucine) */
        11, /* M (methionine) */
        8,  /* N (asparagine) */
        8,  /* P (proline) */
        11, /* Q (glutamine) */
        17, /* R (arginine) */
        5,  /* S (serine) */
        8,  /* T (threonine) */
        10, /* V (valine) */
        18, /* W (tryptophan) */
        15  /* Y (tyrosine) */
    };

    return (seq_t)9 + side_chain_atoms[idx];
}
