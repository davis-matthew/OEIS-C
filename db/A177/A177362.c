/*
 * A177362  a(n) contains the nonzero frequencies f(d) of digits d=0 .. 9 in
 *          all terms up to a(n-1) in concatenated form sorted with respect to
 *          d: f(0)//0//f(1)//1//...//f(9)//9. Initial term a(1)=3.
 * https://oeis.org/A177362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 45,
 *            big any n
 * Helpers:   digit_counts, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177362
 *
 * a(1) = 3.
 * For n>1, let f(d) be the total number of occurrences of digit d
 * in all terms a(1), …, a(n-1) (concatenated in order). Then
 *
 *     a(n) = concat_{d=0}^{9, f(d)>0} ( f(d) followed by d )
 *
 * The function memoises computed terms so that calls may come in
 * any order.  It uses the utilities digit_len, pow10 and digit_counts.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms (cache[1] = a(1), ...) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed = 0;      /* highest index computed so far */

    /* cumulative frequencies of digits 0..9 (uint64_t is large enough) */
    static uint64_t freq[10] = {0};

    /* indices start at 1 */
    if (n == 0) return 0;

    /* ensure the cache is large enough for index n */
    if ((idx_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first term once */
    if (computed == 0) {
        cache[1] = (seq_t)3;              /* a(1) = 3 */
        computed = 1;
        freq[3] = 1;                      /* digit 3 appears once in a(1) */
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t term = 0;                   /* construct a(i) */

        /* concatenate (count, digit) pairs for each digit with non‑zero count */
        for (int d = 0; d < 10; ++d) {
            uint64_t f = freq[d];
            if (f == 0) continue;         /* skip digits with zero frequency */

            seq_t count = (seq_t)f;       /* frequency as seq_t */
            int   digs   = digit_len(count);   /* decimal length of count */

            term = term * pow10((idx_t)digs) + count; /* append the count   */
            term = term * (seq_t)10 + (seq_t)d;       /* append the digit d */
        }

        cache[i] = term;                   /* store the new term */

        /* update cumulative frequencies with digits of 'term' */
        unsigned char add[10] = {0};
        digit_counts(term, add);
        for (int d = 0; d < 10; ++d) {
            freq[d] += (uint64_t)add[d];
        }

        computed = i;
    }

    return cache[n];
}
