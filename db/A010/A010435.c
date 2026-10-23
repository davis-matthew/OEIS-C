/*
 * A010435  Squares mod 74.
 * https://oeis.org/A010435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = r[(n-1) mod 38] + 74 * floor((n-1)/38), where r =
 *            {0,1,3,4,7,9,10,11,12,16,21,25,26,27,28,30,33,34,36,
 *            37,38,40,41,44,46,47,48,49,53,58,62,63,64,65,67,70,71,73} (the
 *            sorted list of quadratic residues modulo 74)
 * Range:     u8 n <= 38, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t residues[38];

    if (!init) {
        /* Compute the 19 distinct residues modulo 37. */
        bool seen[37] = {false};
        for (idx_t i = 0; i < 37; ++i) {
            idx_t v = (i * i) % 37;
            seen[v] = true;
        }

        /* Build the ascending list of quadratic residues mod 74:
           first the residues mod 37, then each plus 37. */
        idx_t pos = 0;
        for (idx_t k = 0; k < 37; ++k) {
            if (seen[k]) {
                residues[pos++] = (seq_t)k;        // k ∈ [0,36]
            }
        }
        /* pos should be 19 */
        for (idx_t j = 0; j < pos; ++j) {
            residues[pos + j] = residues[j] + (seq_t)37;
        }
        init = true;
    }

    idx_t idx = n - 1;                     /* zero‑based index */
    idx_t block = idx / 38;                /* how many full blocks of 74 have passed */
    idx_t offset = idx % 38;               /* position within the block */

    seq_t result = residues[offset];       /* residue from the sorted list  */
    result += (seq_t)(block * 74);         /* add 74×block to get the actual integer */

    return result;
}
