/*
 * A054708  Number of powers of 7 modulo n.
 * https://oeis.org/A054708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct residues 7^k mod n (k ≥ 0)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Compute the size of the set {7^k (mod n)} by simple simulation. */
static bool *seen = NULL;      /* buffer used to mark residues */
static size_t seen_cap = 0;    /* current capacity of 'seen' */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0; harmless guard */

    /* Grow the buffer if necessary and clear it. */
    if (n > seen_cap) {
        bool *tmp = (bool *)malloc((size_t)n);
        if (!tmp) return 0;        /* allocation failure – return 0 */
        free(seen);
        seen   = tmp;
        seen_cap = (size_t)n;
    }
    memset(seen, 0, (size_t)n);    /* reset flags for this call */

    seq_t current = 1 % (seq_t)n;  /* 7^0 mod n */
    seq_t count   = 0;

    while (!seen[(size_t)current]) {
        seen[(size_t)current] = true;
        ++count;
        /* Next power: (current * 7) mod n.  Use seq_t arithmetic. */
        current = (seq_t)((seq_t)current * 7 % (seq_t)n);
    }
    return count;
}
