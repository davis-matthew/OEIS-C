/*
 * A194370  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(2) and < > denotes fractional part.
 * https://oeis.org/A194370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the m-th integer such that * ∑_{k=1..m} ( {½ + k·√2} –
 *            {k·√2} ) > 0 * where {x} is the fractional part of x. * Since {½
 *            + x} – {x} is +½ if {x} < ½ and –½ otherwise, * the condition
 *            reduces to * count_{k≤m}{ {k√2} < ½ } > m/2, * i.e. the
 *            cumulative difference * diff(m) = (# of k with {k√2}...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: integer square root of an unsigned __int128
 * using binary search between 0 and a given upper bound. */
static unsigned long long
isqrt_uint128(unsigned __int128 N, unsigned long long bound)
{
    unsigned long long lo = 0, hi = bound;
    while (lo < hi) {
        unsigned long long mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= N)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cached data for successive calls. */
static seq_t   *terms = NULL;      /* array of terms */
static size_t  terms_len = 0;      /* number of terms stored */
static size_t  terms_cap = 0;      /* allocated capacity */
static idx_t   computed_upto = 0;  /* highest k processed so far */
static long long diff = 0;         /* current cumulative difference */

/* Expand the terms array when needed. */
static void
grow_terms(void)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 4 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; n=0 is not used */

    /* Generate terms until we have at least n entries. */
    while (terms_len < (size_t)n) {
        idx_t k = computed_upto + 1;

        /* Compute floor(k*sqrt(2)) exactly. */
        unsigned __int128 kk = (unsigned __int128)k * k;    /* k² */
        unsigned __int128 two_kk = kk * 2;                 /* 2k² */
        /* sqrt(2) * k = sqrt(2k²), so floor = isqrt(2k²). */
        unsigned long long floor_sqrt = isqrt_uint128(two_kk, (unsigned long long)(2 * k + 2));

        /* Test inequality: 8k² < (2*floor+1)² */
        unsigned __int128 left  = (unsigned __int128)8 * kk;                  /* 8k² */
        unsigned long long g    = 2 * floor_sqrt + 1;
        unsigned __int128 right = (unsigned __int128)g * g;                   /* (2*floor+1)² */

        if (left < right)
            diff++;   /* {k√2} < ½  → +1 */
        else
            diff--;   /* {k√2} ≥ ½  → –1 */

        if (diff > 0) {          /* m = k is a term */
            grow_terms();
            terms[terms_len++] = (seq_t)k;
        }

        computed_upto = k;
    }

    return terms[n - 1];
}
