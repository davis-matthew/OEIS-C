/*
 * A205378  Least k such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=(2j-1)^2.
 * https://oeis.org/A205378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n divides s(k)-s(j) for some j<k
 *            where s(j)=(2j-1)^2. This is equivalent to the first repeated
 *            residue in the sequence of odd squares modulo n. The
 *            implementation iterates k=1,2,… computing r_k = (2k-1)^2 mod n
 *            until a residue repeats
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   bit_get
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bit_get.h"

/* helper: set bit */
static inline void bit_set(uint64_t *bits, uint64_t pos)
{
    bits[pos >> 6] |= 1ULL << (pos & 63);
}

seq_t a(idx_t n)
{
    /* allocate a bit set of size n (number of possible residues) */
    uint64_t sz_bits = (n + 63) >> 6;
    uint64_t *bits = (uint64_t *)calloc(sz_bits, sizeof(uint64_t));
    if (!bits)
        return 0; /* fallback in case of allocation failure */

    seq_t n_seq = (seq_t)n;          /* modulo base, fits in seq_t for practical n */
    seq_t k;                         /* loop counter up to n+1 */

    /* k = 1 */
    seq_t a1 = 1 % n_seq;            /* value of (2*1-1) mod n */
    seq_t res = (seq_t)(((__int128)a1 * a1) % n_seq);
    bit_set(bits, (uint64_t)res);

    for (k = 2; ; ++k) {
        seq_t aval = (seq_t)(((__int128)2 * k - 1) % n_seq);
        res = (seq_t)(((__int128)aval * aval) % n_seq);
        if (bit_get(bits, (uint64_t)res)) {
            free(bits);
            return k;
        }
        bit_set(bits, (uint64_t)res);
    }
}
