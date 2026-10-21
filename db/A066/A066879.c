/*
 * A066879  n such that there are as many 1's as 0's in the base 2 expansion
 *          of Floor(n/2).
 * https://oeis.org/A066879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) are the numbers n such that floor(n/2) has an equal number
 *            of 1's and 0's in its binary expansion
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Count the set bits in a seq_t value. */
static size_t popcount_seq_t(seq_t x)
{
    size_t c = 0;
    while (x) {
        x &= x - 1;   /* clear the lowest set bit */
        ++c;
    }
    return c;
}

/* Storage for already computed terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static seq_t next_m = 1;   /* next m (floor(n/2)) to test */

/* Generate terms until we have at least goal terms. */
static void generate_terms(idx_t goal)
{
    while (terms_len < (size_t)goal) {
        seq_t m = next_m;
        next_m = m + 1;     /* advance for next call */

        unsigned k = bitlen_seq_t(m);
        if ((k & 1) == 0) {                 /* only even bit lengths */
            size_t pop = popcount_seq_t(m);
            if (pop == k / 2) {             /* equal ones and zeros */
                seq_t v1 = m << 1;          /* 2*m   */
                seq_t v2 = v1 + 1;          /* 2*m+1 */

                /* Ensure there is space for two more terms. */
                if (terms_len + 2 > terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
                    if (terms_len + 2 > new_cap)
                        new_cap = terms_len + 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    terms = tmp;
                    terms_cap = new_cap;
                }

                terms[terms_len++] = v1;
                terms[terms_len++] = v2;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard for 0 */
    generate_terms(n);
    return terms[n - 1];
}
