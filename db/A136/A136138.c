/*
 * A136138  a(n+1)=sopfr(4a(n)+1), with sopfr=A001414. Finishes with the cycle
 *          (34, 137, 67, 269, 362, 36).
 * https://oeis.org/A136138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(0) = 1 * a(n+1) = sopfr(4*a(n) + 1) * where sopfr(x) is the
 *            sum of prime factors of x with multiplicity. * The function uses
 *            dynamic memoisation so that subsequent calls are * answered in
 *            O(1) after the necessary terms have been computed
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   sopfr_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sopfr_seq.h"

static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static idx_t capacity = 0;    /* current allocated capacity */
static idx_t size     = 0;    /* number of terms actually stored */

seq_t a(idx_t n)
{
    /* initialise cache with a(0) = 1 */
    if (size == 0) {
        capacity = (idx_t)1;
        cache = (seq_t *)realloc(cache, (size_t)capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        size = (idx_t)1;
    }

    /* extend cache until it contains a(n) */
    while (size <= n) {
        if (size == capacity) {
            capacity = capacity == (idx_t)0 ? (idx_t)1 : capacity * (idx_t)2;
            cache = (seq_t *)realloc(cache, (size_t)capacity * sizeof(seq_t));
        }
        seq_t prev = cache[size - (idx_t)1];
        seq_t x = (seq_t)4 * prev + (seq_t)1;
        cache[size] = sopfr_seq(x);
        size++;
    }

    return cache[n];
}
