/*
 * A200515  Least m>0 such that n = y^2 - 5^x (mod m) has no solution, or 0 if
 *          no such m exists.
 * https://oeis.org/A200515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Computes OEIS sequence A200515. a(n) = least m>0 such that n ≡
 *            y² − 5ˣ (mod m) has no solution, or 0 if no such m exists
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Check whether n can be written as y² − 5ˣ over the integers.
       If so, the sequence value is 0. */
    seq_t nn = (seq_t)n;
    seq_t power = 1;      /* 5^0 */
    for (int x = 0; x <= 20; ++x) {     /* 5^0 … 5^20 : enough for n≤10000 */
        seq_t val = power + nn;
        seq_t s = floor_sqrt(val);
        if (s * s == val) return 0;     /* representable --> a(n)=0 */
        power *= 5;
    }

    /* Find the least modulus m>1 for which n has no solution modulo m. */
    uint64_t un = n;                     /* will be used in % m arithmetic */
    static bool residues[65536];          /* reuse a fixed buffer */

    for (uint64_t m = 2; m < 65536; ++m) {
        memset(residues, 0, (size_t)m);

        /* Pre‑compute all quadratic residues modulo m. */
        for (uint64_t y = 0; y < m; ++y) {
            residues[(y * y) % m] = true;
        }

        /* Iterate over 5ˣ mod m, looking for a matching residue. */
        uint64_t t = 1 % m;   /* 5^0 mod m */
        bool has_solution = false;
        for (uint64_t i = 0; i < m; ++i) {
            uint64_t target = (un + t) % m;
            if (residues[target]) {
                has_solution = true;
                break;
            }
            t = (t * 5) % m;
        }

        if (!has_solution) return (seq_t)m;   /* this m witnesses failure */
    }

    /* For n up to 10 000 the above loop always finds a modulus. */
    return 0;   /* fallback – should never be reached */
}
