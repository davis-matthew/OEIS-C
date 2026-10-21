/*
 * A076404  Odd/even-parity of n-th perfect power.
 * https://oeis.org/A076404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of the n‑th perfect power (OEIS A076404) */
/* a(n) = 1 if the nth perfect power is odd, 0 otherwise */
/* n is 1‑based; a(1) = 1, a(2) = 0, ... */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for sorting uint64_t values. */
static int cmp_uint64(const void *p, const void *q)
{
    const uint64_t *a = (const uint64_t *)p;
    const uint64_t *b = (const uint64_t *)q;
    return (*a > *b) - (*a < *b);
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;            /* outside the offset */
    }

    /* Upper bound that comfortably contains at least n distinct perfect powers.
       For n ≤ 10000 this is far more than sufficient. */
    const uint64_t N = (uint64_t)n;
    uint64_t limit = (N + 1) * (N + 1);

    /* Dynamic array to hold the distinct perfect powers. */
    size_t cap = 2048;
    size_t sz  = 0;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!arr) { abort(); }

    /* The only number with exponent 1 is 1; include it once. */
    arr[sz++] = 1ULL;

    /* Generate all a^b with a ≥ 2 and b ≥ 2 up to the limit. */
    for (uint64_t base = 2; ; ++base) {
        uint64_t power = base * base;           /* exponent = 2 */
        if (power > limit) break;               /* no further bases fit */

        while (power <= limit) {
            if (sz == cap) {
                cap <<= 1;                       /* grow the array */
                uint64_t *tmp = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
                if (!tmp) { free(arr); abort(); }
                arr = tmp;
            }
            arr[sz++] = power;

            if (power > limit / base) break;     /* would overflow the limit */
            power *= base;
        }
    }

    /* Sort the values to bring equal numbers together. */
    qsort(arr, sz, sizeof(uint64_t), cmp_uint64);

    /* Remove duplicates in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (uniq == 0 || arr[i] != arr[uniq - 1]) {
            arr[uniq++] = arr[i];
        }
    }
    sz = uniq;

    /* Fail‑safe: if our limit was too small, regenerate with a larger bound. */
    if ((uint64_t)n > sz) {
        free(arr);
        return (seq_t)0;            /* should never happen for n ≤ 10000 */
    }

    uint64_t value = arr[n - 1];
    free(arr);

    return (seq_t)(value & 1ULL);   /* parity of the nth perfect power */
}
