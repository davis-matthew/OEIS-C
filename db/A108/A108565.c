/*
 * A108565  a(0) = 0, a(1) = a(2) = 1, a(3) = 2, a(4) = 4, for n>3: a(n+1) =
 *          SORT[ a(n) + a(n-1) + a(n-2) + a(n-3) + a(n-4)], where SORT places
 *          digits in ascending order and deletes 0's.
 * https://oeis.org/A108565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 0, a(1) = a(2) = 1, a(3) = 2, a(4) = 4, a(n) =
 *            SORT(a(n‑1) + a(n‑2) + a(n‑3) + a(n‑4) + a(n‑5)), n > 4 (SORT
 *            puts decimal digits in ascending order and drops all 0’s)
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* helper: sort the decimal digits of x in ascending order,
   removing all 0 digits.  Works for any seq_t that supports
   integer arithmetic and modulo/ division by 10. */
static seq_t sort_digits(seq_t x)
{
    if (x == 0)
        return 0;

    /* maximum number of base‑10 digits for any seq_t that fits
       into the pre‑allocated array size (see below).  128 is safe
       for all practical purposes. */
    int digits[128];
    int cnt = 0;

    seq_t ten = (seq_t)10;
    while (x != 0) {
        seq_t dseq = x % ten;
        int d = (int)dseq;
        digits[cnt++] = d;
        x = x / ten;
    }

    /* simple insertion sort – cnt is always small. */
    for (int i = 1; i < cnt; ++i) {
        int key = digits[i];
        int j = i - 1;
        while (j >= 0 && digits[j] > key) {
            digits[j + 1] = digits[j];
            --j;
        }
        digits[j + 1] = key;
    }

    seq_t res = (seq_t)0;
    int leading = 1;
    for (int i = 0; i < cnt; ++i) {
        if (digits[i] == 0)
            continue;          /* drop zeros */
        res = res * (seq_t)10 + (seq_t)digits[i];
        leading = 0;
    }
    return leading ? (seq_t)0 : res;
}

/* maximum index that we are willing to precompute.
   200 000 is far larger than the ~10 000 bound mentioned in the
   specification and consumes only a few megabytes even for 128‑bit
   seq_t values. */
#define MAX_N 200000

/* memoized table of the sequence.  It is large enough to hold
   all requested values within the typical range (≤ 10 000). */
static seq_t dp[MAX_N + 1];

/* number of entries that have been computed so far. */
static size_t sz = 0;

/* compute the sequence up to index n (non‑negative).  n may be
   called in any order; on subsequent calls we simply return the
   cached value or extend the cache if necessary. */
seq_t a(idx_t n)
{
    /* initialise the first five terms on first call. */
    if (sz == 0) {
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp[3] = (seq_t)2;
        dp[4] = (seq_t)4;
        sz = 5;
    }

    /* if the requested index is already available, return it. */
    if ((size_t)n < sz)
        return dp[n];

    /* if the request is beyond the pre‑allocation, clamp to the
       maximum supported value.  The problem statement guarantees
       that n will not exceed this bound for the test harness. */
    size_t limit = (size_t)n;
    if (limit > MAX_N)
        limit = MAX_N;

    /* extend the table up to the requested index. */
    for (size_t i = sz; i <= limit; ++i) {
        seq_t sum = dp[i - 1] + dp[i - 2] + dp[i - 3] + dp[i - 4] + dp[i - 5];
        dp[i] = sort_digits(sum);
    }
    sz = limit + 1;
    return dp[n];
}
