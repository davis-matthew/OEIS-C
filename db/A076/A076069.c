/*
 * A076069  Largest number formed as concatenation of a permutation of the n
 *          consecutive numbers 1+n(n-1)/2, ..., n(n+1)/2 (written in
 *          decimal).
 * https://oeis.org/A076069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less, base
 * Formula:   Computes the maximum concatenation of the consecutive integers
 *            from 1 + n(n-1)/2 to n(n+1)/2 by sorting them with the
 *            comparator that orders x before y iff concat(x,y) > concat(y,x).
 *            The result is obtained by successively appending the sorted
 *            numbers
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   concat_parts, digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

/* Comparator for qsort: put a before b if a+b > b+a. */
static int cmp_num(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t *)p1;
    idx_t b = *(const idx_t *)p2;
    seq_t ab = concat_parts(a, b);
    seq_t ba = concat_parts(b, a);
    if (ab > ba) return -1;   /* a before b */
    if (ab < ba) return 1;    /* b before a */
    return 0;                 /* equal, order doesn't matter */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* range starts at 1, but avoid UB */

    /* Determine the range of consecutive numbers. */
    idx_t start = 1 + (n * (n - 1)) / 2;
    idx_t end   = (n * (n + 1)) / 2;
    size_t count = end - start + 1;    /* should equal n */

    /* Allocate array of the numbers in the range. */
    idx_t *arr = (idx_t *)malloc(count * sizeof(idx_t));
    if (!arr) return 0;                /* allocation failure (unlikely) */
    for (size_t i = 0; i < count; ++i) {
        arr[i] = start + (idx_t)i;
    }

    /* Sort with the custom comparator. */
    qsort(arr, count, sizeof(idx_t), cmp_num);

    /* Concatenate the sorted numbers into a single seq_t value. */
    seq_t res = 0;
    for (size_t i = 0; i < count; ++i) {
        idx_t num = arr[i];
        seq_t mul = pow10((idx_t)digits_of(num));
        res = res * mul + num;
    }

    free(arr);
    return res;
}
