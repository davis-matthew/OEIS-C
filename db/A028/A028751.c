/*
 * A028751  Nonsquares mod 38.
 * https://oeis.org/A028751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Non‑square residues modulo 38 – a(n) is the nth smallest integer r∈[0,37]
   that is not a quadratic residue modulo 38. */
seq_t a(idx_t n)
{
    static bool qr[19];
    static bool ready = false;

    if (!ready) {
        /* Fill the table of quadratic residues modulo 19. */
        for (size_t i = 0; i < 19; ++i)
            qr[(i * i) % 19] = true;
        ready = true;
    }

    /* Scan the residues 0..37, counting the non‑squares. */
    idx_t count = 0;
    for (idx_t r = 0; r < 38; ++r) {
        if (!qr[r % 19]) {
            ++count;
            if (count == n)
                return (seq_t)r;
        }
    }
    /* If n exceeds the number of non‑squares (18), return 0. */
    return (seq_t)0;
}
