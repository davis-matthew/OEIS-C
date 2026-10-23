/*
 * A102823  "True already", base 10, start 0: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 10 contains
 *          floor(a(n)/10) copies of the digit a(n) % 10, with a(0) = 0.
 * https://oeis.org/A102823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the least integer m such that floor(m/10) equals the
 *            number of times the digit (m % 10) occurs in the decimal
 *            representations of a(0), a(1), …, a(n-1). The sequence is
 *            generated iteratively
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* dynamic array holding the sequence and its current length */
static seq_t *seq = NULL;
static idx_t  size = 0;

/* digit occurrence counts for 0‑9 */
static uint64_t counts[10] = {0};

seq_t a(idx_t n)
{
    /* initialise the sequence if it hasn't been built yet */
    if (seq == NULL) {
        seq = (seq_t*)malloc(sizeof(seq_t));
        if (!seq) return (seq_t)0;            /* allocation failure */
        seq[0] = (seq_t)0;                    /* a(0) = 0 */
        size   = 1;
        counts[0] = 1;                        /* "0" contributes one 0‑digit */
    }

    /* the requested term is already computed */
    if (n < size)
        return seq[n];

    /* grow the dynamic array to hold the new terms */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(seq, sizeof(seq_t) * new_size);
    if (!tmp) return (seq_t)0;                /* allocation failure */
    seq = tmp;

    /* compute successive terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t  best = 0;
        bool   first = true;

        for (int d = 0; d < 10; ++d) {
            seq_t cand = (seq_t)counts[d] * (seq_t)10 + (seq_t)d;
            if (first || cand < best) {
                best = cand;
                first = false;
            }
        }
        seq[i] = best;

        /* update digit counts using the newly added term */
        seq_t x = best;
        if (x == 0) {
            counts[0] += 1;
        } else {
            while (x > (seq_t)0) {
                int digit = (int)(x % (seq_t)10);
                counts[digit] += 1;
                x /= (seq_t)10;
            }
        }
    }

    size = new_size;
    return seq[n];
}
