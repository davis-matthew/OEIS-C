/*
 * A178524  Triangle read by rows: T(n,k) is the number of leaves at level k
 *          in the Fibonacci tree of order n (n>=0, 0<=k<=n-1).
 * https://oeis.org/A178524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle recurrence: T(n,k) = T(n-1,k-1)+T(n-2,k-1)
 * With T(0,0)=T(1,0)=1 and T(n,0)=0 for n≥2.
 * The sequence a(n) is obtained by writing the rows of the
 * triangle consecutively (row 0, row 1, row 2, …).  The
 * function below builds the triangle on demand and
 * caches all computed a(n) values.
 */

#include <stdlib.h>

/* --- cached values of a(n) --------------------------------------- */
static seq_t *seq_cache = NULL;   /* a(0), a(1), …, a(last)     */
static idx_t  seq_cap   = 0;
static idx_t  seq_len   = 0;

/* --- two most recent rows of the triangle ------------------------ */
static seq_t *row_prev2 = NULL;   /* row n-2 */
static idx_t  row_prev2_len = 0;
static seq_t *row_prev1 = NULL;   /* row n-1 */
static idx_t  row_prev1_len = 0;

/* initialise seed rows (n = 0 and n = 1) */
static void
init_triangle(void)
{
    if (seq_cache != NULL)
        return;

    seq_cap = 64;
    seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache)
        return;              /* allocation failure – sequence stays empty */

    /* row 0 */
    seq_cache[0] = (seq_t)1;
    /* row 1 */
    seq_cache[1] = (seq_t)1;
    seq_len = 2;              /* indices 0 and 1 are filled */

    /* allocate storage for rows 0 and 1 */
    row_prev2 = (seq_t*)malloc(sizeof(seq_t));
    row_prev2[0] = (seq_t)1;
    row_prev2_len = 1;

    row_prev1 = (seq_t*)malloc(sizeof(seq_t));
    row_prev1[0] = (seq_t)1;
    row_prev1_len = 1;
}

/* extend cached values up to at least index n */
static void
compute_upto(idx_t n)
{
    init_triangle();

    if (seq_len > n)         /* already have a(n) */
        return;

    /* generate rows until a(n) is stored */
    while (seq_len <= n) {
        /* order of the row to produce */
        idx_t new_n   = row_prev1_len + 1;       /* because row_prev1_len = n-1 */
        idx_t new_len = (new_n >= 2) ? new_n : 1;

        seq_t *curr = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!curr)
            return;  /* allocation failure – leave the cache as is */

        /* first entry (k = 0) */
        if (new_n >= 2)
            curr[0] = (seq_t)0;
        else
            curr[0] = (seq_t)1;

        /* remaining entries */
        for (idx_t k = 1; k < new_len; ++k) {
            seq_t v1 = (k-1 < row_prev1_len) ? row_prev1[k-1] : (seq_t)0;
            seq_t v2 = (k-1 < row_prev2_len) ? row_prev2[k-1] : (seq_t)0;
            curr[k] = v1 + v2;
        }

        /* ensure cache capacity for the new values */
        if (seq_len + new_len > seq_cap) {
            idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
            while (new_cap < seq_len + new_len)
                new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return;  /* failure – keep previous cache */
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        /* append the newly computed row to the cache */
        for (idx_t k = 0; k < new_len; ++k)
            seq_cache[seq_len++] = curr[k];

        /* slide the row buffers */
        free(row_prev2);
        row_prev2 = row_prev1;
        row_prev2_len = row_prev1_len;

        row_prev1 = curr;
        row_prev1_len = new_len;
    }
}

/* public entry point ------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_upto(n);
    return seq_cache[n];
}
