/*
 * A064700  Numbers k that are divisible by the multiplicative digital root of
 *          k.
 * https://oeis.org/A064700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 2505, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9803 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = the n‑th natural number with no zero digit that is divisible by its multiplicative digital root
   (product of the decimal digits of the number, repeated until a single digit is obtained). */

#include <stddef.h>                  /* for size_t, NULL */
#include <stdbool.h>
#include <stdlib.h>

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

static inline seq_t mul_digits(seq_t x)
{
    /* Return the product of the decimal digits of x,
       or 0 if x contains a zero digit. */
    seq_t prod = 1;
    seq_t tmp  = x;
    while (tmp > 0) {
        seq_t d = tmp % 10;
        if (d == 0) return 0;
        prod *= d;
        tmp /= 10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoised terms */
    static size_t  cap   = 0;        /* allocated capacity */
    static size_t  sz    = 0;        /* number of terms stored */
    static idx_t   last_checked = 0;  /* last natural number examined */

    if (n == 0) return 0;            /* offset starts at 1 */

    if (n <= sz)                     /* already computed */
        return cache[n - 1];

    /* Generate terms until we have at least n of them */
    for (idx_t cur = last_checked + 1; sz < n; ++cur) {
        seq_t k = (seq_t)cur;

        seq_t mdr = mul_digits(k);
        if (mdr == 0) {              /* number contains a zero digit */
            last_checked = cur;
            continue;
        }

        /* Reduce to a single digit (multiplicative digital root) */
        while (mdr >= 10) {
            seq_t tmp = mul_digits(mdr);
            if (tmp == 0) {          /* root becomes zero during reduction */
                mdr = 0;
                break;
            }
            mdr = tmp;
        }

        if (mdr == 0) {              /* skip if root is zero */
            last_checked = cur;
            continue;
        }

        if (k % mdr == 0) {          /* k is divisible by its MDR */
            if (sz == cap)
                ensure_capacity(sz + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[sz++] = k;
        }

        last_checked = cur;
    }

    return cache[n - 1];
}
