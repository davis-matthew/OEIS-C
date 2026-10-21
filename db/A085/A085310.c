/*
 * A085310  Number of distinct 7th powers modulo n.
 * https://oeis.org/A085310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = |{x^7 mod n : 0 <= x < n}|
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Compute the number of distinct 7th powers modulo n.
 * n ≥ 1.  For n = 0 the function returns 0.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* not defined for n = 0 */

    /* Allocate a bit array to mark which residues have appeared. */
    uint8_t *seen = (uint8_t *)calloc((size_t)n, sizeof(uint8_t));
    if (!seen)
        return (seq_t)0;       /* allocation failure – unlikely for n ≤ 10⁴ */

    /* For each k compute k^7 mod n and mark it. */
    for (idx_t k = 0; k < n; ++k) {
        uint64_t x   = (uint64_t)k % (uint64_t)n;
        uint64_t res = 1;
        uint64_t base = x;
        int e = 7;
        while (e) {
            if (e & 1)
                res = (res * base) % (uint64_t)n;
            base = (base * base) % (uint64_t)n;
            e >>= 1;
        }
        seen[(size_t)res] = 1;
    }

    /* Count distinct residues. */
    seq_t count = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        if (seen[i])
            ++count;
    }

    free(seen);
    return count;
}
