/*
 * A010429  Squares mod 68.
 * https://oeis.org/A010429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010429: Squares mod 68.
   a(n) = the n-th smallest residue r (0 ≤ r < 68) such that
   there exists an integer x with x^2 ≡ r (mod 68). */

#include <stdbool.h>

static seq_t residues[18];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        bool seen[68] = {false};
        seq_t q;
        seq_t mod = 68;
        idx_t i, r;
        /* Mark all residues of i^2 mod 68 for i=0..67 */
        for (i = 0; i < 68; ++i) {
            q = (seq_t)i * (seq_t)i;
            r = (idx_t)(q % mod);
            seen[r] = true;
        }
        /* Collect them in ascending order */
        idx_t pos = 0;
        for (r = 0; r < 68; ++r) {
            if (seen[r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        /* pos should be 18 for modulus 68 */
        (void)pos; /* silence unused warning for static data */
        initialized = true;
    }

    if (n == 0) {
        /* Sequence is defined from offset 1; 0 is undefined. */
        return 0;
    }

    /* n-1 is the index in residues array */
    idx_t idx = n - 1;
    /* If n exceeds the number of residues (finite sequence), return 0 */
    if (idx >= 18) {
        return 0;
    }
    return residues[idx];
}
