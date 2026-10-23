/*
 * A121684  Union of {8, 9, 18}, S, 2S and 4S, where S = odd squarefree
 *          numbers (A056911).
 * https://oeis.org/A121684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest integer in {8,9,18} ∪ odd squarefree numbers
 *            ∪ 2×(odd squarefree) ∪ 4×(odd squarefree)
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data */
    static bool initialized = false;
    static uint8_t *squarefree = NULL;
    const size_t LIMIT = 5000000;         /* sufficient for n up to ~1M            */
    if (!initialized) {
        /* allocate and compute squarefree table up to LIMIT */
        squarefree = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
        for (size_t i = 0; i <= LIMIT; ++i) squarefree[i] = 1;     /* assume squarefree */
        for (size_t p = 2; p * p <= LIMIT; ++p) {
            if (squarefree[p]) {                     /* p is prime or first time seen  */
                size_t p2 = p * p;
                for (size_t j = p2; j <= LIMIT; j += p2)
                    squarefree[j] = 0;                 /* not squarefree */
            }
        }
        initialized = true;
    }

    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        bool member = false;

        /* Check the special elements */
        if (x == 8 || x == 9 || x == 18) {
            member = true;
        } else if ((x & 1) != 0) {                         /* odd */
            /* odd squarefree? */
            member = squarefree[(size_t)x];
        } else {
            if ((x & 3) == 0) {                            /* divisible by 4 */
                idx_t s = x >> 2;                          /* s = x/4    */
                member = (s & 1) && squarefree[(size_t)s];
            } else {                                       /* x%4 == 2   */
                idx_t s = x >> 1;                          /* s = x/2    */
                member = (s & 1) && squarefree[(size_t)s];
            }
        }

        if (member) {
            ++count;
            if (count == n)
                return (seq_t)x;      /* cast into seq_t (unsigned) */
        }
    }
}
