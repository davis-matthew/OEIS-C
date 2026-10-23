/*
 * A130882  a(n) = smallest k such that A002808(n+1) = A002808(n) +
 *          (A002808(n) mod k), or 0 if no such k exists.
 * https://oeis.org/A130882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A130882:
   a(n) = smallest k such that A002808(n+1) = A002808(n) + (A002808(n) mod k),
          or 0 if no such k exists.
   A002808: composite numbers, starting with 4. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Lazy static initialization of composites up to a fixed limit. */
    static bool initialized = false;
    static seq_t *comps = NULL;       /* comps[1] == 4, comps[2] == 6, ... */
    static idx_t comps_cnt = 0;       /* number of composites stored */

    if (!initialized) {
        /* Sieve limit chosen large enough for n up to at least a few
           hundred thousand; for the test range (n <= 10000) 2,000,000 is
           trivial. */
        const idx_t LIMIT = 2000000;
        bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
        if (!is_prime) return 0;  /* allocation failure -> no answer */

        for (idx_t i = 0; i <= LIMIT; i++)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t i = 2; i * i <= LIMIT; i++) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;
            }
        }

        /* Count composites up to LIMIT to pre-allocate array. */
        comps_cnt = 0;
        for (idx_t i = 4; i <= LIMIT; i++)  /* composites start at 4 */
            if (!is_prime[i])
                comps_cnt++;

        comps = (seq_t *)malloc((comps_cnt + 1) * sizeof(seq_t)); /* 1‑based */
        if (!comps) { free(is_prime); return 0; }

        idx_t pos = 1;
        for (idx_t i = 4; i <= LIMIT; i++) {
            if (!is_prime[i]) {
                comps[pos++] = (seq_t)i;
            }
        }

        free(is_prime);
        initialized = true;
    }

    /* Guard against requests beyond pre‑computed range. */
    if (n + 1 > comps_cnt) return 0;  /* undefined behaviour otherwise */

    seq_t m = comps[n];          /* A002808(n)   */
    seq_t next_m = comps[n + 1]; /* A002808(n+1) */

    seq_t d = next_m - m;                   /* difference */
    seq_t N = m - d;                        /* = m − d */

    if (N <= d) return 0;                   /* no divisor > d exists */

    seq_t min_k = 0;
    for (idx_t i = 2; (seq_t)i * (seq_t)i <= N; i++) {
        if (N % (seq_t)i == 0) {
            seq_t i_se = (seq_t)i;
            seq_t j_se = N / i_se;

            if (i_se > d && (min_k == 0 || i_se < min_k))
                min_k = i_se;
            if (j_se > d && (min_k == 0 || j_se < min_k))
                min_k = j_se;
        }
    }
    if (min_k == 0 && N > d) min_k = N;  /* N itself may be the smallest */

    return min_k;
}
