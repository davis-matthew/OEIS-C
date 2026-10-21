/*
 * A077356  Squares whose external digits (MSD and LSD) form a square. Or
 *          squares from which deleting the internal digits leaves a square.
 * https://oeis.org/A077356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077356: squares whose first and last decimal digits form a perfect square. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence offset starts at 1 */

    static seq_t *terms = NULL;           /* cached results   */
    static size_t terms_cap = 0;          /* allocated capacity */
    static size_t terms_len = 0;          /* number of elements stored */
    static uint64_t next_x = 0;           /* next integer to square */

    while (terms_len < (size_t)n) {
        seq_t sq = (seq_t)next_x * (seq_t)next_x;     /* current square   */

        /* first decimal digit of sq */
        seq_t tmp = sq;
        while (tmp >= 10) tmp /= 10;
        int first = (int)tmp;

        /* last decimal digit of sq */
        int last  = (int)(sq % 10);

        /* number formed by external digits */
        int ext = (sq < 10) ? first : first * 10 + last;

        /* check whether ext is a perfect square */
        seq_t root = isqrt_seq((seq_t)ext);
        bool ok = (root * root == (seq_t)ext);

        if (ok) {
            /* grow storage if needed */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 1;
                seq_t *new_ptr = seq_alloc_new_array(new_cap);
                if (terms) seq_free_array(terms);
                terms = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = sq;          /* store result     */
        }
        ++next_x;                              /* advance to next x */
    }

    return terms[n - 1];
}
