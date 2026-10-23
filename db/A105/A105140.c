/*
 * A105140  Numbers n such that 1024n+513 is prime.
 * https://oeis.org/A105140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A105140 – Numbers n such that 1024n+513 is prime.
 *
 * The function finds the n‑th integer m for which 1024*m+513 is a prime
 * number.  Results are cached in a static array so that repeated or out
 * of order calls are inexpensive.  The implementation uses the
 * supplied primality routine and the seq_realloc helper to grow the
 * cache safely.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"   /* for seq_realloc */

seq_t a(idx_t n)
{
    /* cache of found terms */
    static seq_t *terms = NULL;
    static idx_t size  = 0;      /* number of cached terms */
    static idx_t cap   = 0;      /* allocated capacity */
    static idx_t last  = 0;      /* last m examined */

    if (n == 0) return (seq_t)0;       /* offset is 1 – not used */

    /* generate terms until we have at least n of them */
    while (size < n) {
        ++last;
        uint64_t candidate = 1024ULL * last + 513ULL;
        if (is_prime(candidate)) {
            /* grow cache if necessary */
            if (size >= cap) {
                idx_t newcap = cap ? cap * 2ULL : 8ULL;
                seq_t *tmp = seq_realloc(terms, newcap);
                if (!tmp) return (seq_t)0;   /* allocation failed */
                terms = tmp;
                cap   = newcap;
            }
            terms[size++] = (seq_t)last;
        }
    }
    return terms[n - 1];
}
