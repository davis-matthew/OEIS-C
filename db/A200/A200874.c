/*
 * A200874  Number of 0..n arrays x(0..5) of 6 elements without any interior
 *          element greater than both neighbors or less than both neighbors.
 * https://oeis.org/A200874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n^6 + 105*n^5 + 985*n^4 + 2775*n^3 + 3334*n^2 + 1800*n
 *            + 360 ) / 360
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 94, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the exact polynomial given in the OEIS entry.  For n ≤ 7 the
   values are taken from the supplied initial terms.  For larger n the
   polynomial is evaluated using a 128‑bit intermediate when available.
   If 128‑bit integers are not available, a standard recurrence (of order
   7) with memoisation is used as a fallback. */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the function with external linkage */
seq_t a(idx_t n);

static const seq_t INIT[7] = { 26, 163, 602, 1673, 3886, 7973, 14932 };

/* Fallback recurrence implementation for environments without 128‑bit ints */
static seq_t *recurrence_cache = NULL;
static size_t recurrence_cap   = 0;
static size_t recurrence_len   = 0;

/* Helper to grow the recurrence cache */
static void grow_recurrence_cache(size_t needed)
{
    if (needed <= recurrence_cap)
        return;
    size_t new_cap = recurrence_cap ? recurrence_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(recurrence_cache, new_cap * sizeof(seq_t));
    if (!new_arr) /* allocation failure – return zero (undef) */
        return;
    recurrence_cache = new_arr;
    recurrence_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Special case: the first seven terms */
    if (n <= 7)
        return INIT[n - 1];

    /* Use 128‑bit arithmetic when available for the polynomial */
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    {
        __int128 ni = n;
        __int128 num = 0;
        num += ni * ni * ni * ni * ni * ni;          /* n^6 */
        num += (__int128)105 * ni * ni * ni * ni * ni;      /* 105*n^5 */
        num += (__int128)985 * ni * ni * ni * ni;           /* 985*n^4 */
        num += (__int128)2775 * ni * ni * ni;             /* 2775*n^3 */
        num += (__int128)3334 * ni * ni;                  /* 3334*n^2 */
        num += (__int128)1800 * ni;                       /* 1800*n */
        num += 360;                                       /* constant term */
        return (seq_t)(num / 360);
    }
#endif

    /* Fallback: recurrence with memoisation */
    if (!recurrence_cache) {
        recurrence_cap = 8;
        recurrence_cache = (seq_t *)calloc(recurrence_cap, sizeof(seq_t));
        if (!recurrence_cache)
            return 0;   /* allocation failure — return zero */
        for (size_t i = 1; i <= 7; ++i)
            recurrence_cache[i] = INIT[i - 1];
        recurrence_len = 7;
    }

    size_t idx = (size_t)n;
    if (idx <= recurrence_len)
        return recurrence_cache[idx];

    grow_recurrence_cache(idx + 1);

    for (size_t i = recurrence_len + 1; i <= idx; ++i) {
        recurrence_cache[i] =
            7   * recurrence_cache[i - 1]
          - 21  * recurrence_cache[i - 2]
          + 35  * recurrence_cache[i - 3]
          - 35  * recurrence_cache[i - 4]
          + 21  * recurrence_cache[i - 5]
          - 7   * recurrence_cache[i - 6]
          +      recurrence_cache[i - 7];
    }
    recurrence_len = idx;
    return recurrence_cache[idx];
}
