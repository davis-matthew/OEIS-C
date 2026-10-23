/*
 * A028229  Call m Egyptian if we can partition m = x_1+x_2+...+x_k into
 *          positive integers x_i such that Sum_{i=1..k} 1/x_i = 1; sequence
 *          gives all non-Egyptian numbers.
 * https://oeis.org/A028229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   egyptian_rec, gcd_seq, is_egyptian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028229: non‑Egyptian numbers (numbers <78 that cannot be expressed 
   as a sum of reciprocals summing to 1).  The sequence is finite;  
   all such numbers are below 78. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/egyptian_rec.h"
#include "../utils/gcd_seq.h"
#include "../utils/is_egyptian.h"

/* Euclidean algorithm for seq_t */

/* Return the nth non‑Egyptian number (1‑based).  The list is cached for
   subsequent calls. */
seq_t a(idx_t n)
{
    /* All non‑Egyptian numbers are <78. */
    const uint64_t LIMIT = 77;

    static bool initialized = false;
    static seq_t *non = NULL;
    static size_t count = 0;

    if (!initialized) {
        non = (seq_t *)malloc(sizeof(seq_t) * (LIMIT + 1));
        if (non == NULL) {          /* allocation failure – return 0 */
            initialized = true;
            return (seq_t)0;
        }
        count = 0;
        for (uint64_t m = 2; m <= LIMIT; ++m) {
            if (!is_egyptian(m))
                non[count++] = (seq_t)m;
        }
        initialized = true;
    }

    if ((size_t)n > count)
        return (seq_t)0;                            /* out of range */
    return non[n - 1];                              /* 1‑based index */
}
