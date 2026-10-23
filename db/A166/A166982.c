/*
 * A166982  Numbers whose number of ordered factorizations is a perfect power.
 * https://oeis.org/A166982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166982: Numbers whose number of ordered factorizations is a perfect power.
   a(n) = the n‑th integer k (in increasing order) such that A074206(k) is a
   perfect power.  The function below generates the sequence lazily, storing
   previously computed terms in static arrays.  It works for any seq_t/idx_t
   combination supported by the build.  All arithmetic uses only integer
   operations. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* multiply base^exp while staying ≤ limit, return limit+1 on overflow/overflow test */
static inline __int128 pow_leq(__int128 base, int exp, __int128 limit)
{
    __int128 result = 1;
    for (int i = 0; i < exp; ++i) {
        if (base == 0) return 0;                  /* shouldn't happen */
        if (limit / base < result) return limit + 1;
        result *= base;
    }
    return result;
}

/* integer nth root (floor) of x, 2 ≤ n */
static __int128 integer_nth_root(__int128 x, int n)
{
    __int128 low = 2;
    __int128 high = x;
    while (low < high) {
        __int128 mid = low + (high - low + 1) / 2;
        __int128 p = pow_leq(mid, n, x);
        if (p <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* test whether val is a perfect power (≥ 1^2) */
static bool is_perfect_power(__int128 val)
{
    if (val <= 1) return true;            /* 1 = 1^m */

    /* find largest exponent e with 2^e ≤ val */
    int maxExp = 1;
    __int128 pow2 = 2;
    while (pow2 <= val) {
        if (pow2 > val / 2) { pow2 = val + 1; break; }
        pow2 <<= 1;
        ++maxExp;
    }

    /* test all e from 2 to maxExp */
    for (int e = 2; e <= maxExp; ++e) {
        __int128 r = integer_nth_root(val, e);
        if (r > 1 && pow_leq(r, e, val) == val)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;            /* undefined, but safe */

    /* static state */
    static idx_t max_needed = 0u;
    static __int128 *fact = NULL;          /* count of ordered factorizations */
    static size_t fact_size = 0;
    static size_t fact_len = 0;
    static seq_t *qual = NULL;             /* list of qualifying numbers */
    static size_t qual_size = 0;
    static size_t qual_len = 0;
    static idx_t current_k = 1u;           /* last processed integer */

    /* enlarge required count */
    if (n > max_needed) max_needed = n;

    /* initialise on first call */
    if (!fact) {
        fact_size = 16;
        fact = (__int128 *)malloc(fact_size * sizeof(__int128));
        memset(fact, 0, fact_size * sizeof(__int128));
        fact[1] = 1;           /* A074206(1) = 1 */
        fact_len = 1;

        qual_size = 16;
        qual = (seq_t *)malloc(qual_size * sizeof(seq_t));
        qual[0] = 1;           /* 1 is a qualifying number */
        qual_len = 1;
    }

    /* extend computation until we have enough qualifying numbers */
    while (qual_len < max_needed) {
        idx_t k = current_k + 1u;

        /* grow fact array if necessary */
        if ((size_t)k >= fact_size) {
            size_t new_size = fact_size ? fact_size << 1 : 16;
            while (new_size <= (size_t)k) new_size <<= 1;
            fact = (__int128 *)realloc(fact, new_size * sizeof(__int128));
            memset(fact + fact_size, 0, (new_size - fact_size) * sizeof(__int128));
            fact_size = new_size;
        }

        /* compute A074206(k) */
        __int128 sum = 0;
        for (idx_t d = 2; d * d <= k; ++d) {
            if (k % d == 0) {
                sum += fact[k / d]; /* divisor d */
                if (d != k / d)
                    sum += fact[d];  /* divisor k/d */
            }
        }
        sum += fact[1];                /* divisor k */
        fact[k] = sum;
        fact_len = k;
        current_k = k;

        /* test for perfect power and store if true */
        if (is_perfect_power(sum)) {
            if (qual_len >= qual_size) {
                size_t new_size = qual_size ? qual_size << 1 : 16;
                qual = (seq_t *)realloc(qual, new_size * sizeof(seq_t));
                qual_size = new_size;
            }
            qual[qual_len++] = (seq_t)k;
        }
    }

    return qual[n - 1u];   /* indices are 1‑based */
}
