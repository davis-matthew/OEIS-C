/*
 * A212291  Number of permutations of n elements with at most one fixed point.
 * https://oeis.org/A212291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*!n - (-1)^n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: uses the recurrence for subfactorials !n:
   !0 = 1, !1 = 0,
   !n = (n-1)*( !(n-1) + !(n-2) )   for n >= 2
   Then a(n) = 2* !n - (-1)^n,
   i.e. a(n) = 2* !n - 1 if n is even,
          a(n) = 2* !n + 1 if n is odd. */

#include <stdint.h>

#ifdef __cplusplus
/* C++ version: use std::vector for storage */
#include <vector>
static std::vector<seq_t> subfact_vec;   /* subfactorial table */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;          /* convert index to size_t for array ops */

    /* Initialise base cases if not already done */
    if (subfact_vec.empty()) {
        subfact_vec.resize(2);     /* indices 0 and 1 */
        subfact_vec[0] = 1;        /* !0 = 1 */
        subfact_vec[1] = 0;        /* !1 = 0 */
    }

    /* Remember current length before potential growth */
    size_t prev_len = subfact_vec.size();
    if (prev_len <= N) {
        subfact_vec.resize(N + 1); /* extend to hold !N */
    }

    /* Compute subfactorials up to N (if not already computed) */
    for (size_t i = prev_len; i <= N; ++i) {
        seq_t sum = subfact_vec[i - 1] + subfact_vec[i - 2];
        subfact_vec[i] = (seq_t)(i - 1) * sum;
    }

    /* Evaluate a(n) = 2* !n - (-1)^n */
    seq_t res = subfact_vec[N] * 2;
    if ((n & 1ULL) == 0) {          /* n even: (-1)^n =  1 */
        res -= 1;
    } else {                        /* n odd : (-1)^n = -1 */
        res += 1;
    }
    return res;
}

#else
/* C version: use malloc/realloc for storage */
#include <stdlib.h>
static seq_t *subfact = NULL;      /* array of subfactorials */
static size_t subfact_cap = 0;      /* allocated capacity (in entries) */
static size_t subfact_len = 0;      /* number of entries currently valid */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Lazy initialisation: create table with room for indices 0 and 1 */
    if (subfact == NULL) {
        subfact_cap = 2;
        subfact = (seq_t *)malloc(subfact_cap * sizeof(seq_t));
        if (subfact == NULL) { return 0; }   /* allocation failed */
        subfact[0] = 1;                      /* !0 = 1 */
        subfact[1] = 0;                      /* !1 = 0 */
        subfact_len = 2;
    }

    /* Grow capacity if needed */
    if (N + 1 > subfact_cap) {
        size_t new_cap = subfact_cap ? subfact_cap : 1;
        while (N + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(subfact, new_cap * sizeof(seq_t));
        if (tmp == NULL) { return 0; }       /* allocation failed */
        subfact = tmp;
        subfact_cap = new_cap;
    }

    /* Compute subfactorials up to N if not already done */
    for (size_t i = subfact_len; i <= N; ++i) {
        seq_t sum = subfact[i - 1] + subfact[i - 2];
        subfact[i] = (seq_t)(i - 1) * sum;
    }
    subfact_len = N + 1;

    /* Compute a(n) = 2* !n - (-1)^n */
    seq_t res = subfact[N] * 2;
    if ((n & 1ULL) == 0) {          /* even n: subtract 1 */
        res -= 1;
    } else {                        /* odd n : add 1 */
        res += 1;
    }
    return res;
}
#endif
