/*
 * A136403  Absolute values of negative fundamental discriminants (A003657)
 *          that are not 3 mod 4 (A002145).
 * https://oeis.org/A136403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A136403: Absolute values of negative fundamental discriminants (A003657)
   that are not primes ≡ 3 (mod 4) (A002145).  The sequence consists of
   all integers that can be written either

     • 4·d  with d a square‑free integer and d ≡ 1 or 2 (mod 4), or
     • d    with d a square‑free composite integer and d ≡ 3 (mod 4).

   The values are sorted increasingly.  The function returns the n‑th term
   (n ≥ 1) and 0 for n ≤ 0 or n beyond the precomputed range. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison function for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* test if n is square‑free using a precomputed SPF table */
static bool is_squarefree(uint32_t n, const uint32_t *spf)
{
    while (n > 1) {
        uint32_t p = spf[n];
        n /= p;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* sequence implementation */
seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static seq_t *vals = NULL;
    static size_t vals_len = 0;
    static size_t vals_cap = 0;
    static uint32_t *spf = NULL;
    const uint32_t MAX_D = 400000;           /* sufficient for n ≤ 10000 */

    if (!initialized) {
        /* ---- build SPF sieve up to MAX_D ---- */
        spf = (uint32_t *)malloc((MAX_D + 1) * sizeof(uint32_t));
        if (!spf) return (seq_t)0;
        for (uint32_t i = 0; i <= MAX_D; ++i) spf[i] = i;
        for (uint32_t i = 2; i * (uint64_t)i <= MAX_D; ++i) {
            if (spf[i] == i) {
                for (uint32_t j = i * i; j <= MAX_D; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }

        /* ---- preallocate storage for values ---- */
        vals_cap = 250000;                     /* ample capacity */
        vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        if (!vals) return (seq_t)0;

        /* ---- generate sequence values ---- */
        vals_len = 0;
        for (uint32_t d = 1; d <= MAX_D; ++d) {
            if (!is_squarefree(d, spf))
                continue;

            seq_t val;
            if (d % 4 == 3) {
                /* accept only composite numbers (spf[d] != d) */
                if (spf[d] == d)
                    continue;
                val = (seq_t)d;
            } else {
                val = (seq_t)4 * (seq_t)d;
            }

            if (vals_len == vals_cap) {
                vals_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
                if (!tmp) { free(spf); free(vals); return (seq_t)0; }
                vals = tmp;
            }
            vals[vals_len++] = val;
        }

        /* ---- sort to get increasing order ---- */
        qsort(vals, vals_len, sizeof(seq_t), cmp_seq_t);

        initialized = true;
    }

    if (n <= 0 || (idx_t)n > vals_len)
        return (seq_t)0;
    return vals[(size_t)(n - 1)];
}
