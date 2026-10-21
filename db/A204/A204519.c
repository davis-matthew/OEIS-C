/*
 * A204519  Square root of floor(A055851(n)/6).
 * https://oeis.org/A204519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-3) – a(n-6) for n ≥ 8
   Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=1,
                   a(5)=2, a(6)=4, a(7)=11.  */

/* Compute a(n) with memoisation; works for any unsigned seq_t
   (uint8_t … uint64_t, unsigned __int128 or arbitrary‑precision). */

#include <stddef.h>                /* size_t */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t *,size_t,size_t) */

/* static cache (1‑based indexing; index 0 unused) */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;    /* size allocated (ignoring index 0) */
static idx_t  seq_max  = 0;    /* highest index currently cached */

/* ensure seq_cache can hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;              /* one extra slot for index 0 */
    if ((size_t)needed <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while ((size_t)needed > new_cap) new_cap <<= 1;

    seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
    seq_cap = new_cap;
}

/* initial seven values (index 0 unused) */
static const seq_t init_vals[8] = {
    0,      /* dummy for index 0 */
    0,      /* a(1) */
    0,      /* a(2) */
    0,      /* a(3) */
    1,      /* a(4) */
    2,      /* a(5) */
    4,      /* a(6) */
    11      /* a(7) */
};

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* values below offset are undefined */

    if (n > seq_max) {                 /* compute up to n if needed */
        ensure_cache(n);

        for (idx_t i = seq_max + 1; i <= n; ++i) {
            if (i <= 7) {             /* use the predefined constants */
                seq_cache[i] = init_vals[i];
                continue;
            }

            /* recurrence a(i) = 10*a(i-3) - a(i-6) */
            seq_t temp = 10 * seq_cache[i - 3] - seq_cache[i - 6];
            seq_cache[i] = temp;
        }
        seq_max = n;
    }

    return seq_cache[n];
}
