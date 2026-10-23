/*
 * A195637  Number of distinct residues of k^n (mod n), k=0..n-1.
 * https://oeis.org/A195637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = the number of distinct residues of k^n (mod n) for k = 0
 *            … n‑1
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n = 0, return 0 harmlessly */

    /* If n is prime, a(n) = n by Euler's theorem. */
    if (is_prime(n)) return (seq_t)n;

    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;           /* Memory allocation failure: return 0 */

    size_t count = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t residue = mod_pow((seq_t)k, (uint64_t)n, (seq_t)n);
        idx_t r = (idx_t)residue;  /* residue < n guarantees this cast is safe */
        if (!seen[r]) {
            seen[r] = true;
            ++count;
        }
    }

    free(seen);
    return (seq_t)count;
}
