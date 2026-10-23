/*
 * A028803  Nonsquares mod 90.
 * https://oeis.org/A028803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth positive integer that is NOT a quadratic residue
 *            modulo 90 (i.e., there is no integer x with x² ≡ a(n) (mod 90))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute the 90 residues that are quadratic residues modulo 90.
       The array is built once on the first call and reused thereafter. */
    static bool qr[90] = {0};
    static bool init = false;

    if (!init) {
        for (idx_t i = 0; i < 90; ++i) {
            qr[(size_t)((i * i) % 90)] = true;
        }
        init = true;
    }

    idx_t candidate = 1;
    idx_t count = 0;

    /* Find the n-th integer whose residue modulo 90 is not in the set of
       quadratic residues. */
    while (1) {
        if (!qr[(size_t)(candidate % 90)]) {
            ++count;
            if ((idx_t)count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
