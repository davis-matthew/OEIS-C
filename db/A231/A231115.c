/*
 * A231115  Start with 1; then a(n+1) = concatenation of c(d) and d*c(d),
 *          where d is the last digit of a(n) and the counter c(d) is
 *          increased at each occurrence of d, and once more if d*c(d) would
 *          end in zero.
 * https://oeis.org/A231115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n>1, let d be the last digit of a(n-1). Let c(d) be
 *            the number of times d has occurred so far (including this
 *            occurrence). If d*c(d) ends in 0, increase c(d) again. Then
 *            a(n)=concatenation of c(d) and d*c(d)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* static state shared over successive calls */
static seq_t *cache = NULL;      /* cached sequence terms   */
static size_t cache_size = 0;    /* size allocated for cache */
static idx_t cached_n = 0;       /* highest index computed   */
static seq_t counter[10] = { 0 };/* counter c(d) for digits 0‑9 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* allocate or resize cache to hold terms up to n */
    if (cache == NULL) {
        cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache_size = n;
    } else if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = n;
    }

    /* initialise first term if not yet done */
    if (cached_n < 1) {
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cached_n = 1;
        /* counters already zeroed at static initialization */
    }

    /* generate terms from (cached_n+1) up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1)   */
        seq_t d = prev % (seq_t)10;         /* last digit */
        unsigned int di = (unsigned int)d; /* index for counter array */

        /* increment counter for digit d */
        counter[di]++;

        /* compute product d * c(d) */
        seq_t prod = ((seq_t)di) * counter[di];

        /* if resulting product ends in 0, increment counter again */
        if ((prod % (seq_t)10) == (seq_t)0) {
            counter[di]++;
            prod = ((seq_t)di) * counter[di];
        }

        /* concatenate counter and product */
        int prod_digits = digit_len(prod);            /* digits in prod */
        seq_t pow10_val = pow10((idx_t)prod_digits);  /* 10^{digits(prod)} */
        seq_t term = counter[di] * pow10_val + prod;

        /* store new term */
        cache[i - 1] = term;
    }

    cached_n = n; /* update highest index computed */
    return cache[n - 1];
}
