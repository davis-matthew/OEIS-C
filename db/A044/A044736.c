/*
 * A044736  Numbers n such that string 2,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer whose decimal representation
 *   contains the substring "23" but the decimal representation of n+1
 *   does not contain "23".
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains23.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n=0, return 0 */

    static seq_t *terms = NULL;    /* cache of found terms   */
    static size_t count = 0;       /* number of terms cached */
    static size_t capacity = 0;    /* current capacity      */
    static seq_t current = 0;      /* last candidate examined */

    /* grow cache when needed */
    if (capacity == 0) {
        capacity = 64;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    while (count < n) {
        seq_t cand = current + 1;
        current = cand;

        if (contains23(cand)) {
            seq_t nxt = cand + 1;
            if (!contains23(nxt)) {
                if (count == capacity) {            /* enlarge buffer   */
                    capacity <<= 1;
                    terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                }
                terms[count] = cand;
                count++;
            }
        }
    }

    return terms[(size_t)(n - 1)];
}
