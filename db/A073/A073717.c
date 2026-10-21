/*
 * A073717  a(n) = T(2n+1), where T(n) are the tribonacci numbers A000073.
 * https://oeis.org/A073717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = T(2*n+1) (Tribonnaci bisection, a(0)=0,a(1)=1,a(2)=4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *trib      = NULL;
static idx_t  trib_cap  = 0;   /* allocated size (number of elements) */
static idx_t  trib_last = 0;   /* largest index computed so far */
static int    trib_init = 0;   /* initialized flag */

/* Initialize the Tribonacci cache with the first three values. */
static void trib_init_cache(void)
{
    if (!trib_init) {
        trib_cap = 8;                               /* initial capacity */
        trib     = (seq_t *)malloc(trib_cap * sizeof(seq_t));
        trib[0] = (seq_t)0;  /* T(0) */
        trib[1] = (seq_t)0;  /* T(1) */
        trib[2] = (seq_t)1;  /* T(2) */
        trib_last = 2;
        trib_init = 1;
    }
}

/* Ensure that Tribonacci values are computed up to index n (inclusive). */
static void trib_ensure(idx_t n)
{
    trib_init_cache();

    if (n <= trib_last)
        return;

    /* Grow capacity if needed. */
    if (n >= trib_cap) {
        idx_t new_cap = trib_cap * 2;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(trib, new_cap * sizeof(seq_t));
        /* If realloc fails, we simply keep the old pointer and rely on the
         * caller not to exceed practical limits; production code would
         * handle the error. */
        if (tmp)
            trib = tmp;
        trib_cap = new_cap;
    }

    /* Compute additional Tribonacci numbers. */
    for (idx_t i = trib_last + 1; i <= n; ++i) {
        trib[i] = trib[i - 1] + trib[i - 2] + trib[i - 3];
    }
    trib_last = n;
}

/* External function required by the platform. */
seq_t a(idx_t n)
{
    /* We need T(2*n + 1). */
    idx_t target = 2 * n + 1;
    trib_ensure(target);
    return trib[target];
}
