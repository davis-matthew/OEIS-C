/*
 * A127495  Found on geocaching.com, can't find the answer.
 * https://oeis.org/A127495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, unkn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127495: a(1)=23,
   a(n+1) = a(n) + sum_{i=1..n} A007953(a(i))
   where A007953(x) = sum of distinct decimal digits of x */
#include <stddef.h>
#include <stdlib.h>

/* helper: sum of distinct decimal digits of a seq_t value */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = (seq_t)0;
    unsigned int mask = 0;               /* 10 bits for digits 0–9 */
    if (x == (seq_t)0)
        return sum;
    while (x != (seq_t)0) {
        unsigned int d = (unsigned int)(x % (seq_t)10); /* digit (0–9) */
        unsigned int bit = 1U << d;
        if (!(mask & bit)) {           /* first time this digit appears */
            mask |= bit;
            sum += (seq_t)d;
        }
        x /= (seq_t)10;
    }
    return sum;
}

/* cached values of the sequence */
static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cap = 0;        /* allocated size of cache */
static size_t current_max = 0;/* number of values currently stored in cache */
static seq_t S = (seq_t)0;    /* current sum of distinct digits up to current_max */

/* ensure capacity for at least 'want' elements (1‑based) */
static void ensure_capacity(size_t want)
{
    if (want <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < want)
        new_cap = want;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failure: terminate program (no I/O allowed) */
        abort();
    }
    cache = new_cache;
    cap = new_cap;
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* out of range; guard */
        return (seq_t)0;

    if ((size_t)n <= current_max)   /* already computed */
        return cache[(size_t)n - 1];

    /* grow cache to hold n elements */
    ensure_capacity((size_t)n);

    /* initialise if this is the first call */
    if (current_max == 0) {
        cache[0] = (seq_t)23;      /* a(1) */
        current_max = 1;
        /* initialise S = sum_{i=1..1} A007953(a(i)) = digit_sum(a(1)) */
        S = digit_sum(cache[0]);
    }

    /* compute missing terms up to n */
    for (size_t i = current_max; i < (size_t)n; ++i) {
        seq_t new_a = cache[i - 1] + S;   /* a(i+1) = a(i) + S_i */
        cache[i] = new_a;
        S += digit_sum(new_a);           /* S_{i+1} = S_i + digit_sum(a(i+1)) */
    }
    current_max = (size_t)n;
    return cache[(size_t)n - 1];
}
