/*
 * A064362  Numbers k such that no Lucas number is a multiple of k.
 * https://oeis.org/A064362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divides_lucas, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A064362: Numbers k such that no Lucas number is
 * a multiple of k.
 *
 * The algorithm generates all numbers up to a sufficient limit,
 * marks those that divide a Lucas number as “bad”, and
 * propagates the “good” property to all multiples of a good
 * number.  The sequence is then stored in an array so that
 * a(idx_t n) can simply index into it.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/divides_lucas.h"

static bool *good = NULL;     /* true -> good (no Lucas divisor)      */
static seq_t *seq  = NULL;    /* array of sequence values             */
static size_t  seq_len = 0;   /* number of values computed            */
static bool    ready    = false;  /* true when initialized                */

static void init_sequence(void)
{
    /* 200 000 is ample for the first 10 000 terms of A064362
     * (the 10 000‑th term is less than about 1.7 ×10 000). */
    const idx_t LIMIT = 200000;

    good = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!good) abort();

    /* Determine good numbers and mark all their multiples. */
    for (idx_t k = 5; k <= LIMIT; ++k) {
        if (good[k])                 /* already known good (multiple)  */
            continue;
        if (divides_lucas(k))        /* bad: divides a Lucas number   */
            continue;
        /* k is good; mark all its multiples as good. */
        for (idx_t m = k; m <= LIMIT; m += k)
            good[m] = true;
    }

    /* Count good numbers to size the result array. */
    seq_len = 0;
    for (idx_t k = 5; k <= LIMIT; ++k)
        if (good[k])
            ++seq_len;

    seq = seq_alloc_new_array(seq_len);   /* handles big-int types     */
    if (!seq) abort();

    /* Store the good numbers in increasing order. */
    size_t pos = 0;
    for (idx_t k = 5; k <= LIMIT; ++k) {
        if (good[k]) {
            seq[pos++] = (seq_t)k;
        }
    }

    free(good);   /* temporary array no longer needed */
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready)
        init_sequence();

    /* n is guaranteed to be >= 1 by the problem statement. */
    if (n == 0 || (size_t)n > seq_len)
        return (seq_t)0;

    return seq[(size_t)(n - 1)];
}
