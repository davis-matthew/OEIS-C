/*
 * A210225  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210226; see the Formula section.
 * https://oeis.org/A210225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials u(n,x) defined by *
 *            u(1,x)=1, v(1,x)=1 * u(n,x)=x*u(n-1,x)+v(n-1,x)+1 *
 *            v(n,x)=2x*u(n-1,x)+(x+1)*v(n-1,x)+1 * The flattened sequence of
 *            coefficients of u(n,x) with a(1)=1
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the flattened sequence */
    static seq_t *flat = NULL;
    static idx_t flat_count = 0;   /* number of terms stored */
    static idx_t flat_cap = 0;     /* allocated capacity of flat */

    /* static cache of previous row polynomials */
    static seq_t *uprev = NULL;    /* coefficients of u(n-1,x) */
    static seq_t *vprev = NULL;    /* coefficients of v(n-1,x) */
    static idx_t cur_row = 0;      /* number of rows already computed */

    /* initialise on first call */
    if (flat == NULL) {
        flat_cap = 64;
        flat = (seq_t *)malloc(flat_cap * sizeof(seq_t));
        flat_count = 0;

        /* row 1: u(1,x)=1, v(1,x)=1 */
        cur_row = 1;
        uprev = (seq_t *)malloc(sizeof(seq_t));
        vprev = (seq_t *)malloc(sizeof(seq_t));
        uprev[0] = 1;
        vprev[0] = 1;

        /* store coefficients of u(1,x) */
        if (flat_count + 1 > flat_cap) {
            flat_cap = 1;
            flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
        }
        flat[flat_count++] = 1;   /* a(1) = 1 */
    }

    /* generate rows until we have at least n terms */
    while (flat_count < n) {
        idx_t next_row = cur_row + 1;

        /* allocate arrays for new row coefficients */
        seq_t *unew = (seq_t *)malloc((next_row) * sizeof(seq_t));
        seq_t *vnew = (seq_t *)malloc((next_row) * sizeof(seq_t));
        for (idx_t i = 0; i < next_row; ++i) {
            unew[i] = 0;
            vnew[i] = 0;
        }

        /* vnew = 2x*uprev + (x+1)*vprev + 1 */
        for (idx_t i = 0; i < cur_row; ++i) {
            /* 2x*uprev: shift by 1 and add twice */
            vnew[i + 1] += uprev[i];
            vnew[i + 1] += uprev[i];
            /* (x+1)*vprev: x*vprev + vprev */
            vnew[i]     += vprev[i];
            vnew[i + 1] += vprev[i];
        }
        vnew[0] += 1;

        /* unew = x*uprev + vprev + 1 */
        for (idx_t i = 0; i < cur_row; ++i) {
            unew[i + 1] += uprev[i];    /* x*uprev */
        }
        for (idx_t i = 0; i < cur_row; ++i) {
            unew[i] += vprev[i];        /* + vprev */
        }
        unew[0] += 1;

        /* append u(new) coefficients to the flat array */
        if (flat_count + next_row > flat_cap) {
            flat_cap = flat_cap > 0 ? flat_cap * 2 : next_row;
            if (flat_cap < flat_count + next_row)
                flat_cap = flat_count + next_row;
            flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
        }
        for (idx_t i = 0; i < next_row; ++i) {
            flat[flat_count++] = unew[i];
        }

        /* free previous row data and move to new row */
        free(uprev);
        free(vprev);
        uprev = unew;
        vprev = vnew;
        cur_row = next_row;
    }

    return flat[n - 1];
}
