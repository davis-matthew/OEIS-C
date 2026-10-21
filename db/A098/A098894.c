/*
 * A098894  Values of k such that {s(1),...,s(k)} is a palindrome, where
 *          {s(1),s(2),...} is the fixed point of the substitutions 0->1 and
 *          1->110.
 * https://oeis.org/A098894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A098894: lengths k such that the first k symbols of the
    fixed point of 0→1, 1→110 form a palindrome.  The sequence satisfies
    a(1)=1 and for n≥2
        a(n) = a(n-1) + c(ceil(n/2)),
    with c(1)=1, c(2)=3 and for k≥3
        c(k) = 2*c(k-1) + c(k-2).                      */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* index 0 is unused;  indices start at 1 */
    static seq_t *a_cache = NULL;   /* a(i) values               */
    static idx_t  a_last  = 0;      /* largest index computed    */
    static seq_t *c_cache = NULL;   /* c(k) values (k≥1)         */
    static idx_t  c_last  = 0;      /* largest k computed        */

    if (n == 0)                /* undefined for n=0 */
        return 0;

    /* initialise caches on first call */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((2) * sizeof(seq_t));   /* indices 0..1 */
        if (a_cache == NULL) return 0;                   /* allocation failure */
        a_cache[0] = 0;                                 /* unused            */
        a_cache[1] = (seq_t)1;                          /* a(1) = 1          */
        a_last = 1;
    }
    if (c_cache == NULL) {
        c_cache = (seq_t *)malloc((3) * sizeof(seq_t));   /* indices 0..2 */
        if (c_cache == NULL) return 0;                  /* allocation failure */
        c_cache[0] = 0;                                 /* unused            */
        c_cache[1] = (seq_t)1;                          /* c(1) = 1          */
        c_cache[2] = (seq_t)3;                          /* c(2) = 3          */
        c_last = 2;
    }

    /* grow c_cache if needed */
    idx_t kmax = (n + 1) / 2;                     /* ceil(n/2) */
    if (kmax > c_last) {
        size_t new_cap = (size_t)(kmax + 1);
        seq_t *tmp = (seq_t *)realloc(c_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                /* allocation failure */
        c_cache = tmp;
        for (idx_t k = c_last + 1; k <= kmax; ++k) {
            /* c(k) = 2*c(k-1) + c(k-2) */
            seq_t two_prev = c_cache[k - 1] + c_cache[k - 1];
            c_cache[k] = two_prev + c_cache[k - 2];
        }
        c_last = kmax;
    }

    /* grow a_cache if needed */
    if (n > a_last) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                /* allocation failure */
        a_cache = tmp;
        for (idx_t i = a_last + 1; i <= n; ++i) {
            idx_t k = (i + 1) / 2;                /* ceil(i/2) */
            a_cache[i] = a_cache[i - 1] + c_cache[k];
        }
        a_last = n;
    }

    return a_cache[n];
}
