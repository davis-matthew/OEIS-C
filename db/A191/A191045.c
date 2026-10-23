/*
 * A191045  Primes that are squares mod 73.
 * https://oeis.org/A191045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime (excluding 73) that is a quadratic residue
 *            modulo 73
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists primes p > 0 with (p|73) = 1 and p ≠ 73. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Pre‑compute which residues modulo 73 are quadratic residues. */
static bool quad_res[73] = {0};
static bool quad_prepared = false;
static void init_quad_res(void)
{
    if (quad_prepared) return;
    for (int i = 0; i < 73; ++i)
        quad_res[(i * i) % 73] = true;
    quad_prepared = true;
}

/* Return the n-th prime (excluding 73) that is a quadratic residue mod 73. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    init_quad_res();

    idx_t count = 0;
    idx_t p = 2;
    while (1) {
        if (is_prime(p)) {
            if (p != 73 && quad_res[p % 73]) {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }
        ++p;
    }
}
