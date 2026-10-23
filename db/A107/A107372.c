/*
 * A107372  Numbers n such that 103*n - 101 is prime.
 * https://oeis.org/A107372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer n such that 103*n-101 is prime
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* below offset, unused */

    static seq_t *terms = NULL;                 /* table of found terms   */
    static idx_t size  = 0;                     /* number of stored terms */
    static idx_t cap   = 0;                     /* current capacity       */
    static idx_t next  = 0;                     /* last candidate tested  */

    if (size < n) {
        /* Ensure the table is allocated and large enough   */
        if (terms == NULL) {
            cap  = 16;
            terms = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
            next  = 0;
        } else if (cap < n) {
            idx_t new_cap = cap;
            while (new_cap < n) new_cap <<= 1;
            terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }

        /* Generate terms until we have at least n of them   */
        while (size < n) {
            ++next;
            unsigned __int128 prod = (unsigned __int128)next * 103ULL;
            uint64_t m = (uint64_t)(prod - 101ULL);
            if (isPrime64(m)) {
                terms[size++] = (seq_t)next;
            }
        }
    }
    return terms[n - 1];
}
