/*
 * A165794  Nimsum of pairs of consecutive Lucas numbers.
 * https://oeis.org/A165794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003987(Lucas(n), Lucas(n+1)) = Lucas(n) XOR Lucas(n+1)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc() */

seq_t a(idx_t n)
{
    /* static cache of Lucas numbers: lucas[i] holds Lucas(i) */
    static seq_t *lucas      = NULL;
    static size_t lucas_cap  = 0;
    static idx_t  lucas_last = 1;   /* highest index computed so far      */
    static bool    initialized = false;

    /* Ensure at least n+2 entries (indices 0 … n+1) are available. */
    idx_t needed = n + 2;
    if (!initialized) {
        /* First allocation – seq_realloc() handles a NULL old pointer. */
        lucas = seq_realloc(NULL, 0, (size_t)needed);
        lucas_cap = (size_t)needed;
        lucas[0] = 2;   /* Lucas(0) */
        lucas[1] = 1;   /* Lucas(1) */
        initialized = true;
    } else if (lucas_cap < (size_t)needed) {
        lucas = seq_realloc(lucas, lucas_cap, (size_t)needed);
        lucas_cap = (size_t)needed;
    }

    /* Extend cache up to Lucas(n+1). */
    while (lucas_last < (idx_t)(n + 1)) {
        seq_t next = lucas[lucas_last] + lucas[lucas_last - 1];
        ++lucas_last;
        lucas[lucas_last] = next;
    }

    /* Return the n-th term of the sequence. */
    return lucas[n] ^ lucas[n + 1];
}
