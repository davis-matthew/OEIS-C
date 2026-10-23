/*
 * A184987  Nonresidues of k^5 modulo 3331, sorted.
 * https://oeis.org/A184987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 2664, u32 n <= 2664, u64 n <= 2664,
 *            u128 n <= 2664, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonresidues of k^5 modulo 3331, sorted (OEIS A184987) */
#include <stdbool.h>
#include <stdlib.h>

/* Compute the nth nonresidue of k^5 modulo 3331.
   The sequence is finite: there are 2664 nonresidues
   (0 ≤ a < 3331).  For n beyond this range the function
   returns 0. */
seq_t a(idx_t n)
{
    /* static state shared by all calls */
    static bool init = false;
    static seq_t *nonres = NULL;     /* array of nonresidues */
    static size_t nonres_len = 0;    /* number of nonresidues */

    if (!init) {
        const unsigned p = 3331u;
        /* residues[k] is true iff k is a 5th power residue modulo p */
        static bool residues[3331];
        /* mark 0 as a residue */
        residues[0] = true;
        /* compute k^5 mod p for k = 1 .. p-1 */
        for (unsigned k = 1; k < p; ++k) {
            unsigned x   = k;
            unsigned p2  = (x * x) % p;          /* k^2 */
            unsigned p4  = (p2 * p2) % p;        /* k^4 */
            unsigned p5  = (p4 * x) % p;          /* k^5 */
            residues[p5] = true;
        }
        /* count nonresidues */
        nonres_len = 0;
        for (unsigned i = 0; i < p; ++i) {
            if (!residues[i]) ++nonres_len;
        }
        /* allocate array for nonresidues */
        nonres = (seq_t*)malloc(nonres_len * sizeof(seq_t));
        /* fill array in ascending order */
        size_t idx = 0;
        for (unsigned i = 0; i < p; ++i) {
            if (!residues[i]) {
                nonres[idx++] = (seq_t)i;
            }
        }
        init = true;
    }

    /* check bounds */
    if (n == 0 || n - 1 >= nonres_len) {
        return (seq_t)0;
    }
    return nonres[n - 1];
}
