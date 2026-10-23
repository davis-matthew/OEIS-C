/*
 * A047794  a(n) = Sum_{k=0..n} C(n,k)*|Stirling1(n,k)*Stirling2(n,k)|.
 * https://oeis.org/A047794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) *
 *            |Stirling1(n,k)*Stirling2(n,k)|
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ implementation using std::vector for arbitrary‑precision seq_t */
    static std::vector<seq_t> a_cache;
    static std::vector<seq_t> s1_prev, s1_cur;
    static std::vector<seq_t> s2_prev, s2_cur;

    /* Ensure storage for a_cache up to n */
    if (a_cache.size() <= n)
        a_cache.resize(n + 1);

    /* Base case initialization (first call) */
    if (s1_prev.empty()) {
        s1_prev.resize(1);
        s2_prev.resize(1);
        s1_prev[0] = 1;      /* |Stirling1(0,0)| = 1 */
        s2_prev[0] = 1;      /* Stirling2(0,0)   = 1 */
        a_cache[0]   = 1;
    }

    /* Current largest n computed so far */
    idx_t cur = (idx_t)(s1_prev.size() - 1);

    for (idx_t i = cur + 1; i <= n; ++i) {
        /* Make sure buffer rows are large enough */
        if (s1_cur.size() < i + 1) s1_cur.resize(i + 1);
        if (s2_cur.size() < i + 1) s2_cur.resize(i + 1);

        /* Stirling numbers of the first kind (absolute values):
           c(n,k) = c(n-1,k-1) + (n-1)*c(n-1,k) */
        s1_cur[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t mul = s1_prev[k] * (seq_t)(i - 1);
            s1_cur[k] = s1_prev[k - 1] + mul;
        }

        /* Stirling numbers of the second kind:
           S(n,k) = S(n-1,k-1) + k*S(n-1,k) */
        s2_cur[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t mul = s2_prev[k] * (seq_t)k;
            s2_cur[k] = s2_prev[k - 1] + mul;
        }

        /* Sum for a(i) */
        seq_t sum   = 0;
        seq_t binom = 1;                        /* C(i,0) */
        for (idx_t k = 0; k <= i; ++k) {
            seq_t prod = s1_cur[k];
            prod *= s2_cur[k];
            prod *= binom;
            sum += prod;
            if (k < i)
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
        }
        a_cache[i] = sum;

        /* Prepare for next iteration */
        std::swap(s1_prev, s1_cur);
        std::swap(s2_prev, s2_cur);
    }

    return a_cache[n];
#else   /* C implementation */
    /* C implementation using malloc/realloc for standard integer seq_t */
    static seq_t *a_cache  = NULL;
    static size_t a_cap    = 0;
    static size_t a_len    = 0;              /* number of cached terms (next index to compute) */

    /* Two rows for Stirling numbers of the first kind (absolute values). */
    static seq_t *s1_prev = NULL, *s1_cur = NULL;
    /* Two rows for Stirling numbers of the second kind. */
    static seq_t *s2_prev = NULL, *s2_cur = NULL;
    static size_t s_cap   = 0;
    static idx_t  s_len   = 0;              /* largest n computed so far + 1 */

    /* Ensure capacity for a_cache -------------------------------------*/
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : ((size_t)n + 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure (unlikely) */
        a_cache   = tmp;
        a_cap     = new_cap;
    }

    /* Ensure capacity for Stirling buffers --------------------------------*/
    if ((size_t)n + 1 > s_cap) {
        size_t new_cap = s_cap ? s_cap * 2 : ((size_t)n + 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp1 = (seq_t *)realloc(s1_prev, new_cap * sizeof(seq_t));
        seq_t *tmp2 = (seq_t *)realloc(s1_cur,  new_cap * sizeof(seq_t));
        seq_t *tmp3 = (seq_t *)realloc(s2_prev, new_cap * sizeof(seq_t));
        seq_t *tmp4 = (seq_t *)realloc(s2_cur,  new_cap * sizeof(seq_t));
        if (!tmp1 || !tmp2 || !tmp3 || !tmp4) return (seq_t)0;
        s1_prev = tmp1; s1_cur = tmp2;
        s2_prev = tmp3; s2_cur = tmp4;
        s_cap   = new_cap;
    }

    /* Initialize base case n = 0 ----------------------------------------*/
    if (s_len == 0) {
        for (size_t i = 0; i < s_cap; ++i) {
            s1_prev[i] = 0;
            s2_prev[i] = 0;
        }
        s1_prev[0] = 1;      /* |Stirling1(0,0)| = 1 */
        s2_prev[0] = 1;      /* Stirling2(0,0)   = 1 */
        a_cache[0] = (seq_t)1;
        a_len      = 1;
        s_len      = 1;
    }

    /* Compute rows up to n ----------------------------------------------*/
    for (idx_t i = s_len; i <= n; ++i) {
        /* Stirling numbers of the first kind (absolute) */
        s1_cur[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t mul = s1_prev[k] * (seq_t)(i - 1);
            s1_cur[k] = s1_prev[k - 1] + mul;
        }

        /* Stirling numbers of the second kind */
        s2_cur[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t mul = s2_prev[k] * (seq_t)k;
            s2_cur[k] = s2_prev[k - 1] + mul;
        }

        /* Sum for a(i) */
        seq_t sum   = 0;
        seq_t binom = 1;      /* C(i,0) */
        for (idx_t k = 0; k <= i; ++k) {
            seq_t prod = s1_cur[k];
            prod *= s2_cur[k];
            prod *= binom;
            sum += prod;
            if (k < i)
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
        }

        a_cache[(size_t)i] = sum;
        if (a_len <= (size_t)i + 1) a_len = (size_t)i + 1;

        /* Swap current and previous rows for the next iteration */
        seq_t *t;
        t = s1_prev; s1_prev = s1_cur; s1_cur = t;
        t = s2_prev; s2_prev = s2_cur; s2_cur = t;
    }
    s_len = n + 1;
    return a_cache[(size_t)n];
#endif
}
