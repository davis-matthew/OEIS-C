/*
 * A102542  Numbers k such that k333333 is prime.
 * https://oeis.org/A102542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 none, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A102542: numbers k such that k333333 is prime.
   The function returns the n-th such number k.
   It builds the sequence on demand and caches results.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u64.h"

/* dynamic array helpers -------------------------------------------------- */
static seq_t *resize_array(seq_t *old, idx_t old_size, idx_t new_size)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size]();        /* value–init for C++ objects   */
#else
    new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));   /* zero for C */
#endif
    /* copy old data */
    idx_t i;
    for (i = 0; i < old_size && i < new_size; ++i)
        new_arr[i] = old[i];

    /* free old storage */
    if (old) {
#ifdef __cplusplus
        delete[] old;
#else
        free(old);
#endif
    }
    return new_arr;
}

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static idx_t count = 0;   /* number of terms stored   */
    static idx_t allocated = 0; /* allocated size of terms */

    if (n == 0) return 0; /* undefined for n=0, offset is 1 */

    /* if we already know the term, return it */
    if (n <= count) return terms[n - 1];

    /* prepare storage if needed */
    if (allocated == 0) {
        allocated = 64;
        terms = resize_array(NULL, 0, allocated);
    }
    if (count == allocated) {
        allocated <<= 1; /* double */
        terms = resize_array(terms, allocated / 2, allocated);
    }

    /* start searching from the next integer after the last known term */
    idx_t k = (count == 0) ? 10 : (idx_t)terms[count - 1] + 1;

    while (count < n) {
        seq_t val = (seq_t)k * 1000000 + 333333;
        bool prime =
            (sizeof(seq_t) <= 8) ? is_prime_u64((uint64_t)val)
                                 : is_prime_seq(val);

        if (prime) {
            if (count == allocated) {
                allocated <<= 1;
                terms = resize_array(terms, allocated / 2, allocated);
            }
            terms[count++] = (seq_t)k;
        }
        ++k;
    }
    return terms[n - 1];
}
