/*
 * A036111  A summarize Fibonacci sequence starting with a(0)=2 and a(1)=3:
 *          summarize the previous two terms!.
 * https://oeis.org/A036111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is defined recursively as the summary of the two preceding
 *            terms: a(0)=2, a(1)=3, and for n≥2
 *            a(n)=summarize(a(n‑1),a(n‑2)), where summarize concatenates the
 *            decimal digits of the two arguments, counts the occurrences of
 *            each digit, and then writes for each digit d with a positive
 *            count the...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper: produce the summary of two terms */
static seq_t next_term(seq_t a_prev, seq_t a_prev2)
{
    /* Count decimal digits in both terms.  Digit frequency is
       limited to 10 digits (0‑9).  We do not use any external
       library function. */
    unsigned long long freq[10] = {0};
    seq_t t;

    /* Count digits of a_prev */
    t = a_prev;
    while (t != (seq_t)0) {
        unsigned d = (unsigned)(t % (seq_t)10);
        freq[d] += 1;
        t /= (seq_t)10;
    }

    /* Count digits of a_prev2 */
    t = a_prev2;
    while (t != (seq_t)0) {
        unsigned d = (unsigned)(t % (seq_t)10);
        freq[d] += 1;
        t /= (seq_t)10;
    }

    /* Build the result by appending count (in decimal) followed by digit d
       for each d from 9 down to 0. */
    seq_t res = (seq_t)0;
    for (int d = 9; d >= 0; --d) {
        unsigned long long cnt = freq[d];
        if (cnt == 0)
            continue;

        /* Append digits of cnt */
        unsigned long long tmp = cnt;
        unsigned digits[32];
        int len = 0;
        /* cnt>0, so at least one digit */
        while (tmp != 0) {
            digits[len++] = (unsigned)(tmp % 10ULL);
            tmp /= 10ULL;
        }
        for (int i = len - 1; i >= 0; --i)
            res = res * (seq_t)10 + (seq_t)digits[i];

        /* Append the digit itself */
        res = res * (seq_t)10 + (seq_t)d;
    }
    return res;
}

/* Memoized sequence computation */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated cache capacity */

    /* Ensure cache exists and holds at least the first two terms. */
    if (cache_len == 0) {
        /* At least two entries are needed for the base terms. */
        idx_t need = 2;
        if (n + 1 > need)
            need = n + 1;
        /* Allocate or reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)need * sizeof(seq_t));
        cache = tmp;
        cache_cap = need;
        /* Base terms */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* If requested term is already in cache, return it. */
    if (n < cache_len)
        return cache[n];

    /* Extend cache if needed.  We may need up to n+1 entries. */
    if (n + 1 > cache_cap) {
        /* At least accommodate n+1 entries */
        idx_t new_cap = n + 1;
        /* Avoid zero size */
        if (new_cap == 0)
            new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = next_term(cache[i - 1], cache[i - 2]);
    }
    cache_len = n + 1;
    return cache[n];
}
