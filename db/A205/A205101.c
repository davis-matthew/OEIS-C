/*
 * A205101  Least k!! such that n divides k!!-j!! for some j<k.
 * https://oeis.org/A205101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k!! such that n divides (k!! - j!!) for some j < k
   (computed by finding the first repeated residue of double factorials
   modulo n). */

#include <stdlib.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but guard */

    /* visited[res] is 1 if residue 'res' has been seen before */
    size_t len = (size_t)n;          /* safe for n up to 10 000 or so */
    uint8_t *visited = (uint8_t *)calloc(len, sizeof(uint8_t));
    if (!visited)
        return (seq_t)0;  /* out of memory, unlikely in tests */

    seq_t odd_val   = 0, even_val   = 0;
    idx_t odd_res   = 0, even_res   = 0;

    for (idx_t k = 1; ; ++k) {
        idx_t residue;
        seq_t value;

        if (k == 1) {
            residue = 1 % n;
            value   = (seq_t)1;
            odd_val = value;
            odd_res = residue;
        } else if (k == 2) {
            residue = 2 % n;
            value   = (seq_t)2;
            even_val = value;
            even_res = residue;
        } else if (k & 1U) {            /* odd k */
            value   = odd_val * (seq_t)k;
            residue = (idx_t)mod_mul((uint64_t)odd_res, (uint64_t)k,
                                     (uint64_t)n);
            odd_val = value;
            odd_res = residue;
        } else {                        /* even k */
            value   = even_val * (seq_t)k;
            residue = (idx_t)mod_mul((uint64_t)even_res, (uint64_t)k,
                                     (uint64_t)n);
            even_val = value;
            even_res = residue;
        }

        if (visited[residue]) {
            free(visited);
            return value;               /* least k!! found */
        }

        visited[residue] = 1;
    }
}
