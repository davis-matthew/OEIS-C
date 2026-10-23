/*
 * A165909  a(n) is the sum of the quadratic residues of n.
 * https://oeis.org/A165909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct quadratic residues modulo n,
 *            including 0 (i.e., Σ_{k=0}^{n-1} k² mod n, each residue counted
 *            once)
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 just in case */

    /* dynamically sized boolean array for seen residues */
    static uint8_t *seen = NULL;
    static idx_t seen_cap = 0;

    if (n > seen_cap) {
        uint8_t *tmp = (uint8_t *)realloc(seen, (size_t)n);
        if (!tmp) return 0;        /* allocation failed – return 0 */
        seen = tmp;
        seen_cap = n;
    }

    /* reset the array for the current n */
    memset(seen, 0, (size_t)n);

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t r = (i * i) % n;
        if (!seen[r]) {
            seen[r] = 1;
            sum += (seq_t)r;
        }
    }
    return sum;
}
