/*
 * A168541  Numbers consisting of either 2's and 0's or 5's and 0's.
 * https://oeis.org/A168541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth smallest positive integer consisting only of the
 *            digits 0 and 2, or only of the digits 0 and 5. After the first
 *            two terms (2,5) the terms are obtained by merging the two
 *            increasing streams: 2 * m (m has decimal digits 0 or 1 and at
 *            least one 0) 5 * m (m has decimal digits 0 or 1 an...
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of already computed terms   */
static size_t cache_size = 0, cache_cap = 0;   /* cache limits      */

static uint64_t *ms = NULL;        /* breadth‑first queue of numbers
                                     with digits 0 or 1 (positive)   */
static size_t ms_size = 0, ms_cap = 0;
static idx_t ms_pos = 0;           /* next element to expand         */

static seq_t *list2 = NULL;        /* values 2*m (m has 0/1 digits,
                                     at least one zero)                */
static size_t len2 = 0, cap2 = 0;

static seq_t *list5 = NULL;        /* values 5*m (m has 0/1 digits,
                                     at least one zero)                */
static size_t len5 = 0, cap5 = 0;

/* indices of the next element to output from each stream */
static size_t i2 = 0, i5 = 0;

/* ---------------------------------------------------------- */
/* helpers for dynamic arrays                                  */
static void grow(void **p, size_t *cap, size_t *size, size_t elem_size)
{
    if (*size == *cap) {
        size_t new_cap = (*cap) ? (*cap << 1) : 16;
        void *tmp = realloc(*p, new_cap * elem_size);
        if (!tmp) exit(1);
        *p = tmp;
        *cap = new_cap;
    }
}

/* push value to ms queue */
static void push_ms(uint64_t val)
{
    grow((void **)&ms, &ms_cap, &ms_size, sizeof(uint64_t));
    ms[ms_size++] = val;
}

/* push value to list2 */
static void push_list2(seq_t val)
{
    grow((void **)&list2, &cap2, &len2, sizeof(seq_t));
    list2[len2++] = val;
}

/* push value to list5 */
static void push_list5(seq_t val)
{
    grow((void **)&list5, &cap5, &len5, sizeof(seq_t));
    list5[len5++] = val;
}

/* grow cache size */
static void push_cache(seq_t val)
{
    grow((void **)&cache, &cache_cap, &cache_size, sizeof(seq_t));
    cache[cache_size++] = val;
}

/* ---------------------------------------------------------- */
/* test whether a decimal number contains a zero digit */
static int has_zero_digit(uint64_t x)
{
    while (x) {
        if ((x % 10ULL) == 0ULL) return 1;
        x /= 10ULL;
    }
    return 0;
}

/* ---------------------------------------------------------- */
/* generate stream elements until both lists contain at
   least (k+1) elements.  k is a zero based index.          */
static void ensure_lists_up_to(size_t k)
{
    while (len2 <= k || len5 <= k) {
        if (ms_pos >= ms_size) break;          /* should not happen */
        uint64_t cur = ms[ms_pos++];
        if (cur == 1ULL) {
            push_list2((seq_t)2);              /* 2*1 = 2  */
            push_list5((seq_t)5);              /* 5*1 = 5  */
        } else if (has_zero_digit(cur)) {
            push_list2( (seq_t)2 * (seq_t)cur );
            push_list5( (seq_t)5 * (seq_t)cur );
        }
        /* generate children for the BFS queue */
        push_ms(cur * 10ULL);
        push_ms(cur * 10ULL + 1ULL);
    }
}

/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset 1 – undefined */
    if (cache_size >= (size_t)n) return cache[(size_t)(n - 1)];

    /* initialise queues on first call */
    if (ms == NULL) {
        push_ms(1ULL);                  /* start from 1 */
    }

    while (cache_size < (size_t)n) {
        size_t needed = cache_size;     /* zero based index for next term */
        ensure_lists_up_to(needed);     /* guarantee list elements exist */

        seq_t val2 = list2[i2];
        seq_t val5 = list5[i5];

        if (val2 < val5) {
            push_cache(val2);
            i2++;
        } else if (val5 < val2) {
            push_cache(val5);
            i5++;
        } else { /* equal – flush once, skip duplicate */
            push_cache(val2);
            i2++; i5++;
        }
    }
    return cache[(size_t)(n - 1)];
}
