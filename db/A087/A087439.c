/*
 * A087439  Expansion of (1-4x)/((1-x)(1-3x)(1-5x)).
 * https://oeis.org/A087439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*5^n/8 + 3*3^n/4 - 3/8 (OEIS A087439)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence: a(0)=1, a(1)=5, a(2)=22, a(n)=9*a(n-1)-23*a(n-2)+15*a(n-3) for n≥3. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed terms */
static idx_t  len   = 0;      /* number of terms stored */
static idx_t  cap   = 0;      /* capacity of the array */

/* Ensure that cache contains a(0)..a(n).  The array grows geometrically. */
static void ensure_up_to(idx_t n)
{
    if (n < len)          /* already computed */
        return;

    /* Resize array if needed */
    if (n + 1 > cap) {
        size_t new_cap = (cap ? (size_t)cap * 2 : 16);
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;            /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* out of memory */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* Initialize first three terms if missing */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }
    if (len == 1) {
        cache[1] = (seq_t)5;
        len = 2;
    }
    if (len == 2) {
        cache[2] = (seq_t)22;
        len = 3;
    }

    /* Compute remaining terms using the recurrence */
    while (len <= n) {
        /* Compute: 9*a(n-1) - 23*a(n-2) + 15*a(n-3) */
        seq_t term1 = cache[len-1] * (seq_t)9U;
        seq_t term2 = cache[len-2] * (seq_t)23U;
        seq_t term3 = cache[len-3] * (seq_t)15U;
        seq_t val   = term1 - term2;
        val += term3;
        cache[len] = val;
        ++len;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
