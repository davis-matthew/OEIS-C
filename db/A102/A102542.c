/*
 * A102542  Numbers k such that k333333 is prime.
 * https://oeis.org/A102542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 none, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime, isprime_big, resize_array
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
#include "../utils/resize_array.h"
#include "../utils/is_prime.h"       /* bool is_prime(uint64_t)   */
#include "../utils/isprime_big.h"   /* bool isprime_big(seq_t)   */

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
            (sizeof(seq_t) <= 8) ? is_prime((uint64_t)val)
                                 : isprime_big(val);

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
