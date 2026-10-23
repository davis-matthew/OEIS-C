/*
 * A188494  Number of permutations p on the set [n] with the properties that
 *          abs(p(i)-i) <= 3 for all i and p(1) <= 2.
 * https://oeis.org/A188494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            a(n-1)+3*a(n-2)+3*a(n-3)+13*a(n-4)+21*a(n-5)+19*a(n-6)+3*a(n-7)
 *            -7*a(n-8)-9*a(n-9)-5*a(n-10)-3*a(n-11)-3*a(n-12)-a(n-13) for n
 *            >= 13, with initial values
 *            a(0)=0,a(1)=1,a(2)=2,a(3)=4,a(4)=12,a(5)=42,a(6)=138,
 *            a(7)=414,a(8)=1235,a(9)=3764,a(10)=11604,a(11...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;     /* array of computed terms */
static size_t a_cache_len = 0;    /* number of terms stored */

/* first 14 terms of the sequence */
static const seq_t a_init[14] = {
    0,          /* a(0) */
    1,          /* a(1) */
    2,          /* a(2) */
    4,          /* a(3) */
    12,         /* a(4) */
    42,         /* a(5) */
    138,        /* a(6) */
    414,        /* a(7) */
    1235,       /* a(8) */
    3764,       /* a(9) */
    11604,      /* a(10) */
    35664,      /* a(11) */
    109132,     /* a(12) */
    333652      /* a(13) */
};

static void grow_cache(idx_t n)
{
    /* allocate cache up to index n (inclusive) */
    size_t needed = (size_t)n + 1;
    if (needed <= a_cache_len)
        return;
    seq_t *new_cache = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */
    a_cache = new_cache;
    /* initial values up to 13 are known */
    for (size_t i = a_cache_len; i < needed && i < 14; ++i)
        a_cache[i] = a_init[i];
    /* compute remaining terms using recurrence */
    for (size_t i = a_cache_len; i < needed; ++i) {
        if (i < 14)
            continue;
        seq_t pos = a_cache[i-1];
        pos += a_cache[i-2] * 3;
        pos += a_cache[i-3] * 3;
        pos += a_cache[i-4] * 13;
        pos += a_cache[i-5] * 21;
        pos += a_cache[i-6] * 19;
        pos += a_cache[i-7] * 3;
        seq_t neg = a_cache[i-8] * 7;
        neg += a_cache[i-9] * 9;
        neg += a_cache[i-10] * 5;
        neg += a_cache[i-11] * 3;
        neg += a_cache[i-12] * 3;
        neg += a_cache[i-13];
        a_cache[i] = pos - neg; /* non‑negative result */
    }
    a_cache_len = needed;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return a_cache[(size_t)n];
}
