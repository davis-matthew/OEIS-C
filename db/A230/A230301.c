/*
 * A230301  Positive numbers not of the form m + wt(m-1), m >= 1.
 * https://oeis.org/A230301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A010061(n) + 1 (binary self‑numbers, plus one)
 * Range:     u8 n <= 67, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline unsigned int popcount64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* Grow a boolean array of 0/1 values to hold indices up to 'needed' */
static void ensure_capacity(unsigned char **arr, idx_t *cap, idx_t needed)
{
    if (needed <= *cap)
        return;
    idx_t new_cap = (*cap == 0) ? 1024 : (*cap * 2);
    while (new_cap < needed)
        new_cap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(*arr, new_cap);
    if (!tmp)
        abort();                     /* out of memory */
    if (!*arr)
        memset(tmp, 0, new_cap);     /* first allocation zero‑init */
    else
        memset(tmp + *cap, 0, new_cap - *cap);  /* new part zero‑init */
    *arr = tmp;
    *cap = new_cap;
}

/* Mark all forbidden numbers m + popcount(m-1) for m in (last_m .. upto] */
static void ensure_forbidden_upto(idx_t upto,
                                  idx_t *last_m,
                                  unsigned char **forbidden,
                                  idx_t *cap)
{
    for (; *last_m <= upto; ++(*last_m))
    {
        idx_t v = *last_m + popcount64((*last_m) - 1);
        ensure_capacity(forbidden, cap, v + 1);
        (*forbidden)[v] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;           /* how many allowed numbers seen */
    idx_t candidate = 0;       /* integer to test */
    idx_t m = 1;               /* next generator to consider */
    unsigned char *forbidden = NULL;
    idx_t forbidden_cap = 0;

    while (true)
    {
        ++candidate;                          /* next integer to examine */
        ensure_forbidden_upto(candidate, &m, &forbidden, &forbidden_cap);
        ensure_capacity(&forbidden, &forbidden_cap, candidate + 1);

        if (!forbidden[candidate])            /* candidate is allowed */
        {
            ++count;
            if (count == n)
            {
                seq_t result = (seq_t)candidate;
                free(forbidden);
                return result;
            }
        }
    }
}
