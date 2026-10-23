/*
 * A136413  a(1)=1. a(n+1) = a(n) + (number of terms of this sequence that are
 *          <= (1/n)sum{k=1 to n} a(k)).
 * https://oeis.org/A136413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 475, u32 n <= 3247, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1; a(n+1)=a(n)+# {k=1..n | a(k)<=sum_{i=1}^n a(i)/n } */
#include <stdlib.h>

static seq_t *seq = NULL;          /* array holding computed terms */
static idx_t sz   = 0;             /* number of terms currently stored */
static idx_t cap  = 0;             /* current allocation capacity */
static seq_t sum  = 0;             /* sum of terms currently stored */

/* ensure that at least 'needed' elements can be stored */
static void ensure_cap(idx_t needed)
{
    if (needed <= cap) return;
    idx_t new_cap = cap ? cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* count how many of the first 'cnt' terms are <= the average
   (i.e. arr[i] * n <= sum).  The array is sorted in increasing order. */
static idx_t count_leq(const seq_t *arr, idx_t cnt, idx_t n, seq_t sum)
{
    idx_t lo = 0, hi = cnt;                 /* hi is exclusive */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (arr[mid] * (seq_t)n <= sum)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* number of elements <= average */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 as a sentinel */

    /* initialise the first term on the first call */
    if (sz == 0) {
        ensure_cap(1);
        seq[0] = (seq_t)1;
        sum    = (seq_t)1;
        sz     = 1;
        cap    = 1;
    }

    /* compute missing terms up to n (any order of calls supported) */
    for (idx_t i = sz + 1; i <= n; ++i) {
        /* count of terms <= (sum/(i-1)) */
        idx_t cnt = count_leq(seq, i - 1, i - 1, sum);

        seq_t next = seq[i - 2] + (seq_t)cnt;
        ensure_cap(i);
        seq[i - 1] = next;
        sum += next;
    }

    sz = n;
    return seq[n - 1];
}
