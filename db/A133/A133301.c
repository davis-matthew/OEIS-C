/*
 * A133301  a(n) is the n-th pentagonal number which is the sum of two
 *          consecutive pentagonal numbers.
 * https://oeis.org/A133301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133301: a(n) is the n-th pentagonal number which equals
   the sum of two consecutive pentagonal numbers.
   The odd‑indexed terms (a(1), a(3), a(5), …) and the even‑indexed terms
   (a(2), a(4), a(6), …) each satisfy the linear recurrence
        t(k+2) = 1331714*t(k+1) - t(k) - 416160
   where t(1)=a(1)=1, t(2)=a(3)=850137 for the odd subsequence,
   and u(1)=a(2)=1926, u(2)=a(4)=2564464982 for the even subsequence. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *odd_cache  = NULL;   /* stores a(1), a(3), a(5), …  (index 0->a(1)) */
static size_t odd_len      = 0, odd_cap = 0;
static seq_t *even_cache  = NULL;   /* stores a(2), a(4), a(6), …  (index 0->a(2)) */
static size_t even_len      = 0, even_cap = 0;

/* Ensure the odd cache contains at least k elements (k >= 1). */
static void ensure_odd_up_to(size_t k)
{
    if (k <= odd_len) return;
    size_t new_cap = odd_cap ? odd_cap * 2 : 4;
    if (new_cap < k) new_cap = k;
    seq_t *tmp = (seq_t *)realloc(odd_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    odd_cache = tmp; odd_cap = new_cap;

    if (odd_len == 0) {
        odd_cache[0] = (seq_t)1;          /* a(1) */
        odd_len = 1;
    }
    if (odd_len == 1) {
        odd_cache[1] = (seq_t)850137;     /* a(3) */
        odd_len = 2;
    }
    for (size_t i = odd_len; i < k; ++i) {
        seq_t val = odd_cache[i-1] * (seq_t)1331714;
        val = val - odd_cache[i-2] - (seq_t)416160;
        odd_cache[i] = val;
    }
    odd_len = k;
}

/* Ensure the even cache contains at least k elements (k >= 1). */
static void ensure_even_up_to(size_t k)
{
    if (k <= even_len) return;
    size_t new_cap = even_cap ? even_cap * 2 : 4;
    if (new_cap < k) new_cap = k;
    seq_t *tmp = (seq_t *)realloc(even_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    even_cache = tmp; even_cap = new_cap;

    if (even_len == 0) {
        even_cache[0] = (seq_t)1926;        /* a(2) */
        even_len = 1;
    }
    if (even_len == 1) {
        even_cache[1] = (seq_t)2564464982;  /* a(4) */
        even_len = 2;
    }
    for (size_t i = even_len; i < k; ++i) {
        seq_t val = even_cache[i-1] * (seq_t)1331714;
        val = val - even_cache[i-2] - (seq_t)416160;
        even_cache[i] = val;
    }
    even_len = k;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* out of range */

    if ((n & 1ULL) == 0) {               /* even index */
        size_t k = n / 2;                /* k >= 1 for n>=2 */
        ensure_even_up_to(k);
        return even_cache[k-1];
    } else {                             /* odd index */
        size_t k = (n + 1) / 2;          /* k >= 1 for n>=1 */
        ensure_odd_up_to(k);
        return odd_cache[k-1];
    }
}
