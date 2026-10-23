/*
 * A042205  Denominators of continued fraction convergents to sqrt(628).
 * https://oeis.org/A042205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A042205 – denominators of continued‑fraction convergents
 *  to sqrt(628).
 *
 *  Recurrence (for n > 51):
 *      a(n) = 93397457463698 * a(n-26) - a(n-52)
 *
 *  Initial values:
 *      a(0) = 1, a(1) = 16, a(2) = 17, a(3) = 50, a(4) = 267,
 *      a(5) = 1118, a(6) = 2503, a(7) = 3621, a(8) = 9745, a(9) = 23111,
 *      a(10) = 79078, a(11) = 102189, a(12) = 385645, a(13) = 4729929,
 *      a(14) = 14575432, a(15) = 19305361, a(16) = 72491515,
 *      a(17) = 164288391, a(18) = 401068297, a(19) = 565356688,
 *      a(20) = 1531781673, a(21) = 6692483380, a(22) = 34994198573,
 *      a(23) = 76680880526, … (first 52 terms are generated
 *      from the continued fraction of √628).
 *
 *  We memoise the values in a static buffer so that calls in any order
 *  are O(n).  The array is grown only when required.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache    = NULL;
    static idx_t cache_size = 0;

    /* --- Ensure the memoisation buffer can hold n+1 elements --- */
    if (cache == NULL) {                                 /* first call */
        size_t init = (size_t)(n + 1);
        if (init < 52) init = 52;                         /* need first 52 terms */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;                      /* allocation failure */
        cache_size = (idx_t)init;
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
    } else if (cache_size <= n) {                        /* grow if needed */
        size_t new_size = (size_t)(n + 1);
        if (new_size < 52) new_size = 52;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                        /* realloc failure */
        for (idx_t i = cache_size; i < (idx_t)new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* If the requested value is already known, return it */
    if (n < cache_size && cache[n] != (seq_t)0)
        return cache[n];

    /* --- Compute the first 52 terms from the continued fraction of √628
        only once, when any of them is missing. --- */
    if (cache[0] == (seq_t)0) {                           /* not yet computed */
        unsigned long long m   = 0ULL;
        unsigned long long d   = 1ULL;
        const unsigned long long a0 = 25ULL;            /* ⌊√628⌋ */
        unsigned long long a = a0;

        seq_t q_prev2 = (seq_t)1;  /* q(-2) = 1 */
        seq_t q_prev1 = (seq_t)0;  /* q(-1) = 0 */

        for (idx_t i = 0; i <= 51; ++i) {
            if (i > 0) {                                 /* update m,d,a */
                m = d * a - m;
                d = (628ULL - m * m) / d;
                a = (a0 + m) / d;
            }
            seq_t q = (seq_t)a * q_prev1 + q_prev2;      /* q(i) = a(i)*q(i-1)+q(i-2) */
            cache[i] = q;
            q_prev2 = q_prev1;
            q_prev1 = q;
        }
    }

    /* --- Compute remaining terms using the recurrence --- */
    const seq_t coef = (seq_t)93397457463698ULL;
    for (idx_t i = 52; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            cache[i] = coef * cache[i - 26] - cache[i - 52];
        }
    }

    return cache[n];
}
