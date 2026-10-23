/*
 * A103930  Numerators of squares of harmonic numbers A001008/A002805.
 * https://oeis.org/A103930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(H(n)^2) = (numerator(H(n)))^2 where H(n) =
 *            \sum_{k=1}^n 1/k in lowest terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, NULL */

static seq_t gcd_seq_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* not used in OEIS, but defined for safety */
        return 0;

    /* Static memoisation of numerators (hn) and denominators (hd) of H(k) */
    static seq_t *hn = NULL;   /* numerator of H(k) */
    static seq_t *hd = NULL;   /* denominator of H(k) */
    static idx_t  computed = 0;/* maximum k for which hn and hd are set */

    /* Allocate or grow the arrays to size n */
    if (n > computed) {
        seq_t *new_hn = (seq_t *)realloc(hn, n * sizeof(seq_t));
        seq_t *new_hd = (seq_t *)realloc(hd, n * sizeof(seq_t));
        if (!new_hn || !new_hd)   /* allocation failure – return 0 sentinel */
            return 0;
        hn = new_hn;
        hd = new_hd;

        /* Initialise if we have no data yet */
        if (computed == 0) {
            hn[0] = 1;   /* H(1) = 1/1 */
            hd[0] = 1;
            computed = 1;
        }

        /* Extend the known values up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* i is the current index (1‑based) */
            seq_t new_num = hn[i - 2] * (seq_t)i + hd[i - 2];
            seq_t new_den = hd[i - 2] * (seq_t)i;
            seq_t g = gcd_seq_local(new_num, new_den);
            new_num /= g;
            new_den /= g;
            hn[i - 1] = new_num;
            hd[i - 1] = new_den;
        }
        computed = n;
    }

    /* Return the square of the numerator of H(n) */
    seq_t val = hn[n - 1];
    return val * val;
}
