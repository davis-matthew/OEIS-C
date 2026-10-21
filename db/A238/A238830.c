/*
 * A238830  a(1)=a(2)=0; thereafter a(n) = a(n-2)+A238828(n-1)+A238827(n).
 * https://oeis.org/A238830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=0; for n≥3, a(n)=a(n-2)+A238828(n-1)+A238827(n). Using
 *            the generating function given for A238830, the sequence
 *            satisfies the linear recurrence a(n) =
 *            a(n-1)+5*a(n-2)-a(n-3)-7*a(n-4)-a(n-5)+6*a(n-6)+6*a(n-7)+a(n-8)-a(n-9)
 *            for all n ≥ 10. The first nine values are a(1)=0, a(2)=0,
 *            a(3)=0,...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    if (cache == NULL) {
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;  /* a(1) */
        cache[1] = 0;  /* a(2) */
        cache[2] = 0;  /* a(3) */
        cache[3] = 1;  /* a(4) */
        cache[4] = 2;  /* a(5) */
        cache[5] = 6;  /* a(6) */
        cache[6] = 15; /* a(7) */
        cache[7] = 36; /* a(8) */
        cache[8] = 91; /* a(9) */
    }
    if (n <= cap) return cache[n - 1];

    size_t old_cap = cap;
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap = (size_t)n;

    for (idx_t i = old_cap + 1; i <= n; ++i) {
        /* recurrence: a(n)=a(n-1)+5*a(n-2)-a(n-3)-7*a(n-4)-a(n-5)+6*a(n-6)+6*a(n-7)+a(n-8)-a(n-9) */
        cache[i - 1] =
            cache[i - 2]                      /* a(n-1) */
            + 5 * cache[i - 3]                /* 5*a(n-2) */
            - cache[i - 4]                    /* -a(n-3) */
            - 7 * cache[i - 5]                /* -7*a(n-4) */
            - cache[i - 6]                    /* -a(n-5) */
            + 6 * cache[i - 7]                /* +6*a(n-6) */
            + 6 * cache[i - 8]                /* +6*a(n-7) */
            + cache[i - 9]                    /* +a(n-8) */
            - cache[i - 10];                  /* -a(n-9) */
    }

    return cache[n - 1];
}
