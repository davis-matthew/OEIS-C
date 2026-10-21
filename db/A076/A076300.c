/*
 * A076300  Numbers k such that prime(k) + s*k is prime, s=5.
 * https://oeis.org/A076300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that prime(k) + 5 × k is prime (s = 5)
 * Range:     u8 n <= 9, u16 n <= 1539, u32 n <= 58, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *seq = NULL;       /* cached sequence values (k)             */
    static size_t len = 0, cap = 0; /* current length and allocation capacity */
    static idx_t next_k = 1;        /* next k to test (1‑based)              */

    /* Fill the cache until it contains at least n terms.        */
    while (len < n) {
        /* Compute p(k). */
        seq_t p_k = get_prime(next_k);

        /* Compute p(k) + 5 × k. */
        seq_t tmp   = ((seq_t)5) * (seq_t)next_k;
        seq_t cand  = p_k + tmp;

        /* Keep k if the candidate is prime. */
        if (is_prime_big(cand)) {
            grow_seq(&seq, &cap, len + 1); /* ensure space for another term */
            seq[len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    /* Return the requested term (offset 1). */
    return seq[n - 1];
}
