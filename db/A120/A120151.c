/*
 * A120151  a(n) = 5 + floor( Sum_{j=1..n-1} a(j)/3 ).
 * https://oeis.org/A120151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 72, u64 n <= 149,
 *            u128 n <= 304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 5; a(n) = 5 + floor( Σ_{j=1}^{n-1} a(j) / 3 )   (n > 1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of already-computed terms */
    static seq_t *cache = NULL;          /* cache[1 …]  */
    static idx_t max_computed = 0;       /* largest n for which cache is filled */

    /* Sum of terms divided by 3, and remainder modulo 3 of the summed terms.
       We can maintain these two small values instead of the full cumulative sum. */
    static seq_t cur_q = 0;              /* floor(S / 3)  (S = Σ_{j=1}^{k} a(j) at the last computed k) */
    static unsigned int cur_r = 0;       /* S mod 3 */

    /* Undefined for n=0 – return 0 to avoid silent overflow. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache and state on first call.  We use index 1 for a(1). */
    if (max_computed == 0) {
        cache = (seq_t *)malloc((size_t)2 * sizeof(seq_t));   /* allocate indices 0 and 1 */
        if (!cache)
            return (seq_t)0;   /* allocation failure */
        cache[1] = (seq_t)5;            /* a(1) */
        cur_q = (seq_t)1;               /* floor(5 / 3) */
        cur_r = 2;                      /* 5 % 3 */
        max_computed = 1;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed)
        return cache[n];

    /* Resize cache to hold terms up to n. */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* If reallocation fails, keep old cache (caller may interpret 0 as error). */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute missing terms, using the recurrence and the stored quotient/remainder. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t next = (seq_t)5 + cur_q;      /* a(i) = 5 + floor(S(i-1)/3) */
        cache[i] = next;

        /* Update S(i) = S(i-1) + a(i) using quotient/remainder tracking. */
        seq_t add = (seq_t)cur_r + next;    /* current remainder plus a(i) */
        seq_t add_q = add / (seq_t)3;       /* floor((R + a(i))/3) */
        cur_q += add_q;                     /* new floor(S(i)/3) */
        cur_r = (unsigned int)(add % (seq_t)3); /* new S(i) mod 3 */
    }

    max_computed = n;
    return cache[n];
}
