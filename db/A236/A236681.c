/*
 * A236681  Positive integers a such that there exist integers b, c > 0 with
 *          1/a + 1/b + 1/c = 1/2.
 * https://oeis.org/A236681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   has_solution_for_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) are positive integers for which ∃ b,c>0 with
   1/a + 1/b + 1/c = 1/2.  The values are obtained by enumerating
   all a ≥ 3 and testing the existence of suitable integers b,c. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_solution_for_a.h"

static seq_t *cache = NULL;      /* memoised sequence values     */
static idx_t  size  = 0;           /* number of values stored      */
static idx_t  capacity = 0;        /* current allocation capacity */

/* Build the sequence once.  The loop stops when a>200
   because all solutions occur for a≤42; 200 is a safe safety margin. */
static void init_cache(void)
{
    if (cache != NULL) return;              /* already initialised   */

    capacity = 20;                          /* initial capacity      */
    cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    size  = 0;

    for (idx_t a = 3; a <= 200; ++a) {
        if (has_solution_for_a(a)) {
            if (size == capacity) {         /* grow if needed      */
                capacity *= 2;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[size++] = (seq_t)a;
        }
    }
}

/* Public function as required by the build system. */
seq_t a(idx_t n)
{
    init_cache();                           /* ensure table exists  */
    if (n == 0) return 0;                  /* undefined offset     */
    if (n <= size)                          /* valid index          */
        return cache[n - 1];
    else
        return 0;                            /* no further terms     */
}
