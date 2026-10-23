/*
 * A145137  Expansion of x/((1 - x - x^4)*(1 - x)^8).
 * https://oeis.org/A145137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   expansion of x/((1 - x - x^4)*(1 - x)^8). Recurrence (order 12):
 *            a(n) = 9 a(n-1) -36 a(n-2) +84 a(n-3) -125 a(n-4) +118 a(n-5)
 *            -56 a(n-6) -20 a(n-7) +61 a(n-8) -55 a(n-9) +28 a(n-10) -8
 *            a(n-11) + a(n-12) Initial values: a(0)=0, a(1)=1, a(2)=9,
 *            a(3)=45, a(4)=165, a(5)=496, a(6)=1297, a(7)=3058, a(...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 39, u64 n <= 108, u128 n <= 246,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache   = NULL;   /* cached values of a(n) */
static size_t a_cap    = 0;       /* allocated capacity of a_cache */
static idx_t  a_len    = 0;       /* number of values currently stored (>=0) */

/* Ensure that a_cache[0..n] are computed. */
static void ensure_a(idx_t n)
{
    /* Allocate/extend cache if needed */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap ? (a_cap << 1) : 16;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Fill the first 12 terms if this is the first call. */
    if (a_len == 0) {
        /* Make sure capacity is at least 12 */
        if (a_cap < 12) {
            seq_t *tmp = (seq_t*)realloc(a_cache, 12 * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            a_cap   = 12;
        }
        a_cache[0]  = (seq_t)0;
        a_cache[1]  = (seq_t)1;
        a_cache[2]  = (seq_t)9;
        a_cache[3]  = (seq_t)45;
        a_cache[4]  = (seq_t)165;
        a_cache[5]  = (seq_t)496;
        a_cache[6]  = (seq_t)1297;
        a_cache[7]  = (seq_t)3058;
        a_cache[8]  = (seq_t)6655;
        a_cache[9]  = (seq_t)13586;
        a_cache[10] = (seq_t)26323;
        a_cache[11] = (seq_t)48829;
        a_len = (n < 12 ? n + 1 : 12);
    }

    /* Compute any new terms not yet computed. */
    idx_t start = a_len;
    if (start < 12) start = 12;          /* first new term that needs recurrence */
    for (idx_t k = start; k <= n; ++k) {
        seq_t term = (seq_t)0;
        term += (seq_t)9   * a_cache[k-1];
        term -= (seq_t)36  * a_cache[k-2];
        term += (seq_t)84  * a_cache[k-3];
        term -= (seq_t)125 * a_cache[k-4];
        term += (seq_t)118 * a_cache[k-5];
        term -= (seq_t)56  * a_cache[k-6];
        term -= (seq_t)20  * a_cache[k-7];
        term += (seq_t)61  * a_cache[k-8];
        term -= (seq_t)55  * a_cache[k-9];
        term += (seq_t)28  * a_cache[k-10];
        term -= (seq_t)8   * a_cache[k-11];
        term += a_cache[k-12];
        a_cache[k] = term;
    }
    a_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[(size_t)n];
}
