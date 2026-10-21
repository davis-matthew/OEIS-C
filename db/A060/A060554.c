/*
 * A060554  String together the first n numbers in an order which maximizes
 *          the result.
 * https://oeis.org/A060554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum integer obtainable by concatenating
   the numbers 1..n in some order – i.e. the lexicographically
   largest concatenation.  The sequence is defined by sorting the
   integers 1,2,…,n so that for any two numbers a and b, the
   pair a,b is preferred to b,a if ab > ba.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digits_of.h"

/* Compare two indices a and b so that the concatenation a followed
   by b is larger than b followed by a.  If ab > ba, a should come
   before b. */
static int cmp_desc(const void *p, const void *q)
{
    idx_t a = *(const idx_t *)p;
    idx_t b = *(const idx_t *)q;
    int da = digits_of(a);
    int db = digits_of(b);

    /* compute 10^da and 10^db using 64‑bit arithmetic */
    uint64_t pow_da = 1, pow_db = 1;
    for (int i = 0; i < da; ++i) pow_da *= 10;
    for (int i = 0; i < db; ++i) pow_db *= 10;

    uint64_t ab = (uint64_t)a * pow_db + (uint64_t)b;
    uint64_t ba = (uint64_t)b * pow_da + (uint64_t)a;

    if (ab > ba) return -1;      /* a before b */
    if (ab < ba) return 1;       /* b before a */
    return 0;                   /* equal, keep order */
}

/* Concatenate the first n positive integers in the order that maximizes
   the resulting integer and return that value. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Build array 1..n */
    idx_t *arr = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!arr) return 0;             /* allocation failure */
    for (idx_t i = 0; i < n; ++i) arr[i] = i + 1;

    /* Sort using the custom comparator */
    qsort(arr, (size_t)n, sizeof(idx_t), cmp_desc);

    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t val = arr[i];
        int d = digits_of(val);

        /* 10^d as seq_t */
        seq_t mult = 1;
        for (int j = 0; j < d; ++j) mult *= (seq_t)10;

        /* result = result * mult + val;  works for arbitrary precision */
        result = result * mult + (seq_t)val;
    }

    free(arr);
    return result;
}
