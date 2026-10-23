/*
 * A110897  a(n) = 1 + a(n - a(n-1)) - (n - a(n-1)) mod 2 + (n - a(n-2)) mod
 *          3, with a(0) = 0, a(1) = 1.
 * https://oeis.org/A110897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 + a(n - a(n-1)) - (n - a(n-1)) mod 2 + (n - a(n-2)) mod
 *            3 a(0) = 0, a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static data for memoisation */
static seq_t *seq      = NULL;   /* values */
static unsigned char *comp = NULL; /* computed flag */
static size_t seq_cap   = 0;      /* allocated size */

/* ensure that the tables can hold indices up to n */
static void ensure_cap(idx_t n)
{
    if ((size_t)n + 1 <= seq_cap) return;

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 8) new_cap = 8;

    if (!seq) {
        seq = (seq_t*)malloc(new_cap * sizeof(seq_t));
        comp = (unsigned char*)malloc(new_cap * sizeof(unsigned char));
    } else {
        seq  = (seq_t*)realloc(seq,  new_cap * sizeof(seq_t));
        comp = (unsigned char*)realloc(comp, new_cap * sizeof(unsigned char));
    }
    if (!seq || !comp)   /* fatal, bail out */
        exit(EXIT_FAILURE);

    /* initialise new cells */
    for (size_t i = seq_cap; i < new_cap; ++i) {
        seq[i] = 0;
        comp[i] = 0;
    }

    /* set base values */
    seq[0] = 0;
    seq[1] = 1;
    comp[0] = comp[1] = 1;

    seq_cap = new_cap;
}

/* sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    ensure_cap(n);

    /* compute only missing entries */
    for (size_t i = 2; i <= (size_t)n; ++i)
    {
        if (comp[i]) continue;          /* already computed */

        seq_t a_nm1 = seq[i-1];
        seq_t a_nm2 = seq[i-2];

        idx_t ia_nm1 = (idx_t)a_nm1;
        idx_t ia_nm2 = (idx_t)a_nm2;

        idx_t diff1 = (idx_t)i - ia_nm1; /* n - a(n-1) */
        idx_t diff2 = (idx_t)i - ia_nm2; /* n - a(n-2) */

        seq_t mod1 = (seq_t)(diff1 & 1);   /* diff1 mod 2 */
        seq_t mod2 = (seq_t)(diff2 % 3);   /* diff2 mod 3 */

        seq_t term = 1 + seq[diff1] - mod1 + mod2;
        seq[i] = term;
        comp[i] = 1;
    }

    return seq[(size_t)n];
}
