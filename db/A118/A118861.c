/*
 * A118861  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 14.
 * https://oeis.org/A118861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; For n>1, let x = a(n−1), d = most significant digit of
 *            x, then a(n) = ((x % 10^(d−1)) * 10 + d) + 14 (i.e. move the
 *            first digit to the end and add 14)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"   /* int digit_len(seq_t x); */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */

static seq_t *cache = NULL;     /* memoized sequence values */
static size_t cap   = 0;        /* allocated capacity */
static idx_t len    = 0;        /* number of stored terms (n ≥ 1) */

/* Ensure `cache` can hold at least `need` terms. */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Return a(n) for the sequence A118861. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; out of bounds -> 0 */

    /* Initialise first term if needed. */
    if (len == 0) {
        ensure_capacity(1);
        cache[0] = 1;
        len = 1;
    }

    /* If the requested term is already cached, return it. */
    if (n <= len) return cache[n - 1];

    /* Compute terms up to n, caching results. */
    seq_t curr = cache[len - 1];
    for (idx_t idx = len; idx < n; ++idx) {
        /* digit count of current term */
        int d = digit_len(curr);
        seq_t pow = pow10(d - 1);      /* 10^(d-1) */
        seq_t first = curr / pow;      /* most significant digit */
        seq_t without = curr % pow;    /* remaining digits */
        seq_t next = without * 10 + first + 14;
        ensure_capacity(idx + 1);
        cache[idx] = next;
        curr = next;
    }
    len = n;
    return cache[n - 1];
}
