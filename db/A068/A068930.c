/*
 * A068930  Number of incongruent ways to tile a 5 X 2n room with 1x2 Tatami
 *          mats. At most 3 Tatami mats may meet at a point.
 * https://oeis.org/A068930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) * - a(n-7) - a(n-8) - a(n-9) *
 *            a(1) = 4, a(2) = 2, a(3) = 1, * a(4) = 1, a(5) = 1, a(6) = 2, *
 *            a(7) = 2, a(8) = 3, a(9) = 3, * a(10) = 5, a(11) = 5 * (For n ≥
 *            12 the recurrence applies.)
 * Range:     u8 n <= 25, u16 n <= 45, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- internal state ----------------------------------------------*/
static seq_t *seq = NULL;          /* array of computed terms */
static idx_t  max_alloc = 0;       /* allocated capacity (size)   */
static idx_t  computed = 0;        /* highest index for which seq[i] is valid */

/*--- ensure we have space for indices up to n --------------------*/
static void ensure_capacity(idx_t n)
{
    if (n < max_alloc) return;                /* already enough space */

    idx_t new_cap = (max_alloc > 0) ? max_alloc : 64;
    while (new_cap <= n) new_cap <<= 1;        /* grow geometrically */
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    max_alloc = new_cap;
}

/*--- initialise the static array with the known first terms ------*/
static void init_terms(void)
{
    if (seq) return;                          /* already initialised */

    /* Allocate initial capacity */
    max_alloc = 64;
    seq = (seq_t *)malloc(max_alloc * sizeof(seq_t));

    /* dummy zero at index 0 for easier indexing */
    seq[0] = 0;

    /* Known terms, 1‑based */
    seq[1] = 4;
    seq[2] = 2;
    seq[3] = 1;
    seq[4] = 1;
    seq[5] = 1;
    seq[6] = 2;
    seq[7] = 2;
    seq[8] = 3;
    seq[9] = 3;
    seq[10] = 5;
    seq[11] = 5;

    computed = 11;                            /* highest term defined */
}

/*--- compute all terms up to n -----------------------------------*/
static void compute_up_to(idx_t n)
{
    if (n <= computed) return;                /* nothing to do */

    ensure_capacity(n);

    /* Extend sequence from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 11) {
            /* values already set by init_terms() */
            continue;
        }
        seq[i] = seq[i - 2] + seq[i - 3] + seq[i - 4]
               - seq[i - 7] - seq[i - 8] - seq[i - 9];
    }
    computed = n;
}

/*--- public API -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    init_terms();                             /* ensure array ready */

    compute_up_to(n);                         /* fill missing values */

    return seq[n];
}
