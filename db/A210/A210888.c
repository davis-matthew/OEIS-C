/*
 * A210888  Dates after Jan 01 00 in chronological order which are palindromic
 *          when they are written in the format DD.MM.YY. The terms are listed
 *          as numbers (without the dots). Leading zeros of the terms are
 *          suppressed.
 * https://oeis.org/A210888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 7 Initial values (offset
 *            = 1): a(1)=101101, a(2)=201102, a(3)=301103, a(4)=11110,
 *            a(5)=111111, a(6)=211112, a(7)=21120 The values are generated on
 *            demand and memoised
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Maximum number of terms pre‑allocated. 200 000 is ample for n ≤ 10 000. */
#define A210888_MAX 200000

/* Static cache for previously computed terms. */
static seq_t  a_cache[A210888_MAX];
static size_t a_cache_len = 0;   /* number of terms currently stored */

/* ------------------------------------------------------------------ */
/* Public function:  seq_t a(idx_t n)                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Guard against n == 0 (offset starts at 1). */
    if (n == 0)
        return 0;   /* undefined for index 0 */

    /* Prime the cache with the 7 known initial terms. */
    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)101101;   /* a(1) */
        a_cache[1] = (seq_t)201102;   /* a(2) */
        a_cache[2] = (seq_t)301103;   /* a(3) */
        a_cache[3] = (seq_t)11110;    /* a(4) */
        a_cache[4] = (seq_t)111111;   /* a(5) */
        a_cache[5] = (seq_t)211112;   /* a(6) */
        a_cache[6] = (seq_t)21120;    /* a(7) */
        a_cache_len = 7;
    }

    /* If the requested index is already cached, return immediately. */
    if ((idx_t)a_cache_len >= n)
        return a_cache[n - 1];

    /* Otherwise, extend the cache up to index n using the recurrence. */
    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        /* Indices: a(i) = a(i-1) + a(i-3) - a(i-4) */
        a_cache[i - 1] =
            a_cache[i - 2]          /* a(i-1) */
          + a_cache[i - 4]          /* a(i-3) */
          - a_cache[i - 5];         /* a(i-4) */
    }
    a_cache_len = (size_t)n;

    return a_cache[n - 1];
}
