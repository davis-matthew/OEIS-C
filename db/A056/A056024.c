/*
 * A056024  Numbers k such that k^10 == 1 (mod 11^2).
 * https://oeis.org/A056024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 5415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   init_base_terms, mod_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056024 – Numbers k such that k^10 ≡ 1 (mod 11²)
 *
 *  a(n) = a(n‑10) + 121      for n ≥ 11
 *  a(1)…a(10) are the 10 solutions in 1…120:
 *      1, 3, 9, 27, 40, 81, 94, 112, 118, 120
 *  Thus a(n) = base[(n‑1) mod 10] + 121 * ⌊(n‑1)/10⌋
 */

/* minimal includes – idx_t and seq_t are supplied externally */
#include <stdint.h>
#include "../utils/init_base_terms.h"
#include "../utils/mod_pow10.h"

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t base[10];
    static int   initialized = 0;

    if (!initialized) {
        init_base_terms(base);
        initialized = 1;
    }

    /* index into the base array (0‑based) */
    idx_t r = (n - 1) % 10;
    idx_t q = (n - 1) / 10;          /* number of 121‑blocks */

    /* term = base[r] + 121 * q   (all arithmetic in seq_t) */
    return base[r] + ((seq_t)q * (seq_t)121);
}
