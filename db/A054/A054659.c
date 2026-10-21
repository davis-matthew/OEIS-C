/*
 * A054659  Increasing sequence with no repeating digits and no digits shared
 *          with previous term.
 * https://oeis.org/A054659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   valid_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the smallest integer greater than a(n-1) that has
 * no repeated digits and shares no digit with a(n-1); a(1) = 0.
 * This implementation generates terms on demand and caches them
 * in a static array. The array size accommodates the required range
 * (n up to ~10000).  It works with any seq_t/idx_t types supported
 * by the build: unsigned integer types or a C++ arbitrary‑precision
 * integer class.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/valid_next.h"

/* Maximum number of terms that this implementation can cache.
   The sequence grows slowly; 200 000 is more than enough for
   n up to about 10 000 and keeps the runtime well below the limit. */
#define MAX_SEQ 200000

/* Static cache of sequence values. */
static seq_t seq[MAX_SEQ];
static size_t seqLen = 0;

/* Compute a(n) for 1‑based index n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but return 0 as a safe fallback */

    /* Ensure we have generated terms up to n. */
    while (seqLen < (size_t)n) {
        bool prevSeen[10] = {0};
        seq_t prev = seqLen ? seq[seqLen - 1] : 0;

        /* For the first term, any digit is allowed (prevSeen all false). */
        if (seqLen > 0) {
            seq_t tmp = prev;
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 10);
                prevSeen[d] = true;
                tmp /= 10;
            }
        }

        seq_t cand = (seqLen == 0) ? 0 : (prev + 1);

        /* Find the next valid candidate. */
        while (!valid_next(cand, prevSeen)) {
            ++cand;
        }

        /* Store the candidate. */
        seq[seqLen++] = cand;
    }

    return seq[n - 1];
}
