/*
 * A085429  Numbers k such that the number of 1's in the binary expansion of k
 *          is equal to the number of 1's in the binary expansion of the
 *          reversal of k.
 * https://oeis.org/A085429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085429 – Numbers k such that popcount(k)==popcount(rev(k)).
   Computed by scanning integers in order and selecting those whose binary
   popcount equals that of their decimal reversal. */
#include <stdlib.h>
#include "../utils/rev.h"

/* static cache of found terms */
static seq_t *seq      = NULL;   /* array of terms */
static size_t capacity = 0;      /* current allocation size */
static size_t count    = 0;      /* number of terms stored */
static idx_t cur       = 0;      /* next integer to test */

/* ensure capacity for 'needed' elements */
static void grow(size_t needed)
{
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity * 2 : 32;
        while (new_cap < needed) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }
}

/* popcount for seq_t (works for any unsigned integral or arbitrary‑precision type) */
static unsigned int popcount_seq(seq_t x)
{
    unsigned int c = 0;
    while (x) {            /* popcount by repeated clearing of lowest set bit */
        x &= x - 1;
        c++;
    }
    return c;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n+1 entries */
    while (count <= (size_t)n) {
        /* test current number cur (idx_t underlying type) */
        unsigned int pc_cur  = popcount_seq((seq_t)cur);
        idx_t        rev_cur = rev(cur);            /* decimal reversal   */
        unsigned int pc_rev = popcount_seq((seq_t)rev_cur);

        if (pc_cur == pc_rev) {
            /* store as seq_t (explicit cast to correct type) */
            if (count >= capacity) grow(count + 1);
            seq[count] = (seq_t)cur;
            count++;
        }
        cur++;   /* next integer to test */
    }
    return seq[(size_t)n];
}
