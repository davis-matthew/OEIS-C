/*
 * A210224  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210223; see the Formula section.
 * https://oeis.org/A210224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(n,x)=x*u(n-1,x)+v(n-1,x)+1, v(n,x)=2x*u(n-1,x)+x*v(n-1,x)+1,
 *            with u(1,x)=v(1,x)=1. a(n) is the n‑th coefficient of the
 *            triangle of v(n,x), read row by row
 * Range:     u8 n <= 33, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Off‑by‑one safety. */

    /* Base row: n = 1. */
    seq_t *Uprev = (seq_t*)malloc(sizeof(seq_t));
    seq_t *Vprev = (seq_t*)malloc(sizeof(seq_t));
    if (!Uprev || !Vprev) return 0;       /* Allocation failure. */
    Uprev[0] = 1;
    Vprev[0] = 1;

    if (n == 1) {
        seq_t r = Vprev[0];
        free(Uprev); free(Vprev);
        return r;
    }

    /* start is the 1‑based index of the first coefficient in the
       current row. After the first row, it is 2. */
    size_t start = 2;          /* Index of first coefficient in row 2. */

    for (idx_t row = 2; ; ++row) {
        size_t cur_len = (size_t)row;          /* Length of current row. */
        size_t end_idx = start + cur_len - 1;  /* Inclusive end index. */

        /* Does the desired term lie in this row? */
        if ((size_t)n <= end_idx) {
            /* Allocate the current row.  We only need it when returning. */
            seq_t *Ucur = (seq_t*)malloc(cur_len * sizeof(seq_t));
            seq_t *Vcur = (seq_t*)malloc(cur_len * sizeof(seq_t));
            if (!Ucur || !Vcur) {
                free(Ucur); free(Vcur); free(Uprev); free(Vprev);
                return 0;
            }

            /* Compute the row. */
            for (size_t k = 0; k < cur_len; ++k) {
                if (k == 0) {
                    Ucur[0] = Vprev[0] + 1;
                    Vcur[0] = 1;
                } else {
                    seq_t uprev_k_1 = Uprev[k - 1];
                    seq_t vprev_k    = (k < row-1) ? Vprev[k] : 0; /* previous row length is row-1 */
                    Ucur[k] = uprev_k_1 + vprev_k;

                    seq_t vprev_k_1 = Vprev[k - 1];
                    Vcur[k] = 2 * uprev_k_1 + vprev_k_1;
                }
            }

            /* Return the requested coefficient. */
            seq_t result = Vcur[(size_t)n - start];
            free(Ucur); free(Vcur); free(Uprev); free(Vprev);
            return result;
        }

        /* Desired term not in this row: compute the row to continue. */
        seq_t *Ucur = (seq_t*)malloc(cur_len * sizeof(seq_t));
        seq_t *Vcur = (seq_t*)malloc(cur_len * sizeof(seq_t));
        if (!Ucur || !Vcur) {
            free(Ucur); free(Vcur); free(Uprev); free(Vprev);
            return 0;
        }

        for (size_t k = 0; k < cur_len; ++k) {
            if (k == 0) {
                Ucur[0] = Vprev[0] + 1;
                Vcur[0] = 1;
            } else {
                seq_t uprev_k_1 = Uprev[k - 1];
                seq_t vprev_k    = (k < row-1) ? Vprev[k] : 0;
                Ucur[k] = uprev_k_1 + vprev_k;

                seq_t vprev_k_1 = Vprev[k - 1];
                Vcur[k] = 2 * uprev_k_1 + vprev_k_1;
            }
        }

        /* Prepare for the next iteration. */
        free(Uprev); free(Vprev);
        Uprev = Ucur;
        Vprev = Vcur;
        start += cur_len;   /* Start index of the next row. */
    }
}
