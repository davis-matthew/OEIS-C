/*
 * A025180  a(n) = number of (s(0), s(1), ..., s(n)) such that s(i) is an
 *          integer, s(0) = 0, |s(1)| = 1, |s(i) - s(i-1)| <= 1 for i >= 2,
 *          s(n) = 2. Also a(n) = T(n,n-2), where T is the array defined in
 *          A025177.
 * https://oeis.org/A025180
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2)=1, a(3)=2, a(4)=7, for n≥5: a(n) = [2*(2*n+1)*(n-3)*a(n-1)
 *            + (-n^2+10*n-18)*a(n-2) - 3*(2*n-3)*(n-3)*a(n-3)] /
 *            ((n+2)*(n-3))
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Choose an intermediate type large enough for multiplication */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 int128_t;
#else
typedef unsigned long long int128_t;
#endif

/* Memoisation storage */
static seq_t *a_cache   = NULL;   /* array of computed a(i) */
static size_t  a_cap    = 0;      /* capacity of a_cache (max index + 1) */
static idx_t   a_next   = 2;      /* next index yet to compute */

/* Helper: grow cache if needed */
static void a_ensure_capacity(idx_t n)
{
    if (a_cache == NULL) {
        a_cap = n + 1;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (a_cache == NULL) {
            /* out of memory – give up */
            a_cap = 0;
            return;
        }
    } else if (a_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;      /* allocation failed – keep old buffer */
        a_cache = tmp;
        a_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* below offset */

    a_ensure_capacity(n);
    if (a_cache == NULL) return (seq_t)0;  /* allocation failed */

    for (idx_t i = a_next; i <= n; ++i) {
        if (i == 2) {
            a_cache[i] = (seq_t)1;
        } else if (i == 3) {
            a_cache[i] = (seq_t)2;
        } else if (i == 4) {
            a_cache[i] = (seq_t)7;
        } else {
            /* coefficients using 64‑bit arithmetic */
            unsigned long long coeff1 = 2ULL * (2ULL * (unsigned long long)i + 1ULL)
                                        * (i - 3);
            long long V = -(long long)i * (long long)i
                          + 10LL * (long long)i - 18LL;
            unsigned long long coeff3 = 3ULL * (2ULL * (unsigned long long)i - 3ULL)
                                        * (i - 3);
            unsigned long long denom = (unsigned long long)(i + 2)
                                       * (i - 3);

            /* intermediate terms (128‑bit if available) */
            int128_t term1 = (int128_t)a_cache[i - 1] * coeff1;
            int128_t term3 = (int128_t)a_cache[i - 3] * coeff3;
            int128_t term2_abs = (int128_t)a_cache[i - 2] *
                                 (unsigned long long)(V >= 0 ? V : -V);

            int128_t numerator;
            if (V >= 0) {
                numerator = term1 + term2_abs - term3;
            } else {
                numerator = term1 - term3 - term2_abs;
            }

            a_cache[i] = (seq_t)(numerator / (int128_t)denom);
        }
    }

    a_next = n + 1;
    return a_cache[n];
}
