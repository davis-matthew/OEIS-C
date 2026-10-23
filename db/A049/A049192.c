/*
 * A049192  Start with 1. Read as base 10, convert to base 2, describe it in
 *          base 2.
 * https://oeis.org/A049192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary look‑and‑say sequence starting with 1. a(1) =
 *            1 For n > 1: read a(n‑1) as a decimal integer, convert it to
 *            binary, then run‑length encode that binary string, writing
 *            counts (in binary) followed by the bit value, and use the
 *            resulting bit string as the next decimal number
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   decimal_to_binary, mkstr, run_length_encode, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/decimal_to_binary.h"
#include "../utils/mkstr.h"
#include "../utils/run_length_encode.h"

/* Cache of computed terms (stored as decimal strings). */
static char **cached_terms = NULL;
static size_t cached_size  = 0;

/* Main sequence function. */
seq_t a(idx_t n) {
    if (!cached_terms) {
        /* Allocate room for index 1. */
        cached_size = 2;
        cached_terms = (char **)malloc(sizeof(char *) * cached_size);
        cached_terms[1] = mkstr("1");
    }

    if ((size_t)n >= cached_size) {
        /* Expand cache. */
        cached_size = (size_t)n + 1;
        cached_terms = (char **)realloc(cached_terms, sizeof(char *) * cached_size);
        for (size_t i = 2; i <= n; ++i) {
            char *bin = decimal_to_binary(cached_terms[i - 1]);
            char *next = run_length_encode(bin);
            free(bin);
            cached_terms[i] = next;
        }
    }

    return seq_from_str(cached_terms[n]);
}
