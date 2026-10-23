/*
 * A180850  Number of codons that code for an amino acid, listed in
 *          alphabetical order of their single-letter codes.
 * https://oeis.org/A180850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of RNA codons that encode the n‑th amino acid
 *        in alphabetical order of its single‑letter code.
 *        (a(1) is for 'A' (Ala), a(2) for 'B' (none), …)
 *        For n > 26 the value is 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state: initialise once */
    static bool initialized = false;
    static seq_t count[26];   /* counts for 'A'..'Z' */

    if (!initialized) {
        /* Standard genetic code: 64 codons mapping to amino acids.
           Nucleotides are ordered A, C, G, U in each position. */
        static const char codon_table[64] = {
            /* N=AAA..AAU */
            'K','N','K','N',
            /* N=ACA..ACU */
            'T','T','T','T',
            /* N=AGA..AGU */
            'R','S','R','S',
            /* N=AUA..AUU */
            'I','I','M','I',
            /* N=CAA..CAU */
            'Q','H','Q','H',
            /* N=CCA..CCU */
            'P','P','P','P',
            /* N=CGA..CGU */
            'R','R','R','R',
            /* N=CUA..CUU */
            'L','L','L','L',
            /* N=GAA..GAU */
            'E','D','E','D',
            /* N=GCA..GCU */
            'A','A','A','A',
            /* N=GGA..GGU */
            'G','G','G','G',
            /* N=GUA..GUU */
            'V','V','V','V',
            /* N=UAA..UAU */
            0 , 'Y', 0 , 'Y',
            /* N=UCA..UCU */
            'S','S','S','S',
            /* N=UGA..UGU */
            0 , 'C', 'W','C',
            /* N=UUA..UUU */
            'L','F','L','F'
        };

        /* Count codons for each amino acid (ignore stop codons, value 0) */
        for (idx_t i = 0; i < 64; ++i) {
            char aa = codon_table[i];
            if (aa >= 'A' && aa <= 'Z') {
                ++count[(size_t)(aa - 'A')];
            }
        }
        initialized = true;
    }

    if (n < 1) return 0;
    /* convert to 0‑based index */
    if (n > 26) return 0;
    return count[(size_t)(n - 1)];
}
