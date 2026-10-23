/*
 * A028750  Nonsquares mod 37.
 * https://oeis.org/A028750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 37 * floor((n-1)/18) + the ((n-1) mod 18)-th quadratic
 *            non‑residue modulo 37 (n≥1)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1 */

    /* cache of the 18 quadratic non‑residues modulo 37 */
    static seq_t NONRES[18];
    static bool initialized = false;

    if (!initialized) {
        idx_t count = 0;
        for (idx_t x = 1; x <= 36; ++x) {
            seq_t val = mod_pow((seq_t)x, (uint64_t)18, (seq_t)37);
            if (val != (seq_t)1)   /* Legendre symbol (x|37) ≠ 1 ⇒ non‑residue */
                NONRES[count++] = (seq_t)x;
        }
        initialized = true; /* NONRES now holds the 18 residues in ascending order */
    }

    const idx_t PER = 18;
    idx_t q = (n - 1) / PER;               /* block number */
    unsigned int r = (unsigned int)((n - 1) % PER); /* offset within the block */

    seq_t result = (seq_t)q * (seq_t)37 + NONRES[r];
    return result;
}
