/*
 * A121579  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n having along the lower contour exactly k reentrant
 *          corners, i.e., a vertical step that is followed by a horizontal
 *          step (n>=1, k>=0).
 * https://oeis.org/A121579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle read by rows, T(n,k) given by P(n,t) = Q(n,t,1),
 *            Q(1,t,x)=1, Q(n,t,x)=Q(n-1,t,t)+(n-1)x Q(n-1,t,1) The 1‑D
 *            sequence a(n) is the flattened triangle, indexed from 1. The
 *            function below computes a(n) for any n≥1, memoising previously
 *            computed entries. The algorithm is O(max_row^2) where max...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*------------------------------------------------------------*/
    /*  Static state – keeps everything from one call to the next  */
    /*------------------------------------------------------------*/
    static seq_t *flat = NULL;           /* flattened sequence (1‑based)   */
    static size_t flat_len = 0;           /* highest index stored           */
    static size_t flat_cap = 0;           /* capacity of flat array         */

    static seq_t *A1 = NULL, *B1 = NULL; /* first pair of row buffers      */
    static seq_t *A2 = NULL, *B2 = NULL; /* second pair of row buffers     */
    static seq_t *A_prev = NULL, *B_prev = NULL;
    static seq_t *A_cur  = NULL, *B_cur  = NULL;
    static size_t A_prev_len = 0, B_prev_len = 0;

    static idx_t last_row = 0;           /* number of rows already generated */

    /* ----------------------------------------------------------- */
    /*  Initialise static buffers on first call                    */
    /* ----------------------------------------------------------- */
    if (!flat)
    {
        /* maximum row length needed for n up to 10000 is 5001 */
        const size_t MAX_LEN = 6000;     /* a small safety margin */

        A1 = (seq_t*)malloc(MAX_LEN * sizeof(seq_t));
        A2 = (seq_t*)malloc(MAX_LEN * sizeof(seq_t));
        B1 = (seq_t*)malloc(MAX_LEN * sizeof(seq_t));
        B2 = (seq_t*)malloc(MAX_LEN * sizeof(seq_t));
        if (!A1 || !A2 || !B1 || !B2) return 0;  /* out of memory */

        A_prev = A1;  B_prev = B1;
        A_cur  = A2;  B_cur  = B2;

        /* start with row 1 */
        A_prev[0] = 1;
        B_prev[0] = 1;
        A_prev_len = 1;
        B_prev_len = 1;

        /* flat[0] is unused; make room for at least one element  */
        flat_cap = 16;
        flat = (seq_t*)calloc(flat_cap + 1, sizeof(seq_t));
        if (!flat) return 0;
        flat[1] = 1;
        flat_len = 1;

        last_row = 1;
    }

    /* ----------------------------------------------------------- */
    /*  Grow the flattened array when necessary                    */
    /* ----------------------------------------------------------- */
    if (n > flat_len)
    {
        /* ensure flat has enough capacity for the new terms */
        while (flat_len < n)
        {
            idx_t r = last_row + 1;           /* next row index         */
            seq_t m = (seq_t)(r - 1);         /* multiplier              */

            /* sizes for new row */
            size_t newA_len = B_prev_len;     /* equals len(B_prev)      */
            size_t newB_len = A_prev_len + 1;

            /* ------------------ compute A_cur ------------------- */
            size_t k;
            for (k = 0; k < newA_len; ++k)
            {
                seq_t val = B_prev[k];
                if (k < A_prev_len)
                    val += m * A_prev[k];
                A_cur[k] = val;
            }

            /* ------------------ compute B_cur ------------------- */
            B_cur[0] = B_prev[0];
            for (k = 1; k < newB_len; ++k)
            {
                seq_t val = 0;
                if (k < B_prev_len)
                    val += B_prev[k];
                val += m * A_prev[k-1];
                B_cur[k] = val;
            }

            /* ----------------- append A_cur to flat ------------- */
            /* grow flat if needed */
            if (flat_len + newA_len > flat_cap)
            {
                size_t new_cap = flat_cap * 2;
                if (new_cap < flat_len + newA_len)
                    new_cap = flat_len + newA_len;
                seq_t *tmp = (seq_t*)realloc(flat, (new_cap + 1) * sizeof(seq_t));
                if (!tmp) return 0;
                flat = tmp;
                flat_cap = new_cap;
            }

            for (k = 0; k < newA_len; ++k)
            {
                flat[flat_len + 1] = A_cur[k];
                ++flat_len;
            }

            /* ----------------- swap row buffers --------------- */
            seq_t *tmp = A_prev;  A_prev = A_cur;  A_cur = tmp;
            tmp = B_prev;         B_prev = B_cur;  B_cur = tmp;
            A_prev_len = newA_len;
            B_prev_len = newB_len;

            ++last_row;
        }
    }

    /* ----------------------------------------------------------- */
    /*  Result is now available in the static cache               */
    /* ----------------------------------------------------------- */
    return flat[n];
}
