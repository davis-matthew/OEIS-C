/*
 * A010446  Squares mod 85.
 * https://oeis.org/A010446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest quadratic residue modulo 85 (i.e. the n-th
 *            smallest non‑negative integer r with r ≡ k² (mod 85) for some k)
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static data to avoid recomputing the residue set */
    static bool ready = false;
    static seq_t residues[85];
    static idx_t rescount;

    if (!ready) {
        bool used[85] = {0};
        /* mark residues of i² modulo 85 for i = 0 … 84 */
        for (idx_t i = 0; i < 85; i++) {
            seq_t r = ((seq_t)i * (seq_t)i) % (seq_t)85;
            used[(int)r] = true;
        }
        /* collect them in ascending order */
        idx_t k = 0;
        for (idx_t i = 0; i < 85; i++) {
            if (used[(int)i]) {
                residues[k++] = (seq_t)i;
            }
        }
        rescount = k;      /* number of distinct residues (27 for modulus 85) */
        ready = true;
    }

    /* the OEIS offset is 1 */
    if (n == 0 || n > rescount)
        return 0;          /* undefined beyond the set; returning 0 is harmless */
    return residues[n - 1];
}
