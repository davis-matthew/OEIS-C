/*
 * A210796  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210795; see the Formula section.
 * https://oeis.org/A210796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle of coefficients of v(n,x) *
 *            computed from the recurrences: * * u(n,x) = u(n-1,x) +
 *            x*v(n-1,x) + 1 * v(n,x) = (x+2)*u(n-1,x) + (x-1)*v(n-1,x) * with
 *            u(1,x)=1, v(1,x)=1 * * The sequence a(n) enumerates the
 *            coefficients of v(n,x) row by row: * row 1: 1 * row 2: 1,2 *
 *            r...
 * Range:     u8 n <= 40, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t for idx_t */

seq_t a(idx_t n) {
    /* Return value for a(1)=1 and for n<=0 use 0 (out of range). */
    if (n == 0) return 0;

    /* Static state kept between calls. */
    static idx_t built_row    = 1;          /* last row computed */
    static seq_t *Uprev       = NULL;      /* coefficients of u(n-1,x) */
    static seq_t *Vprev       = NULL;      /* coefficients of v(n-1,x) */
    static size_t Ulen        = 0;          /* length of Uprev   */
    static size_t Vlen        = 0;          /* length of Vprev   */
    static seq_t *flat        = NULL;      /* flattened coefficients */
    static size_t flat_len    = 0;          /* number of terms stored */

    /* Initialise on first call. */
    if (built_row == 1 && Uprev == NULL) {
        /* Row 1 : u(1)=1 , v(1)=1 */
        Ulen = Vlen = 1;
        Uprev = (seq_t*)malloc(sizeof(seq_t));
        Vprev = (seq_t*)malloc(sizeof(seq_t));
        Uprev[0] = 1;
        Vprev[0] = 1;

        /* Flattened list starts with the sole coefficient of v(1,x). */
        flat_len = 1;
        flat = (seq_t*)malloc(sizeof(seq_t));
        flat[0] = 1;
    }

    /* Build further rows until we have at least n terms. */
    while (flat_len < n) {
        idx_t next_row = built_row + 1;      /* number of coefficients in next row */
        size_t next_len = (size_t)next_row;

        /* Allocate arrays for u(n,x) and v(n,x). */
        seq_t *Unext = (seq_t*)malloc(sizeof(seq_t) * next_len);
        seq_t *Vnext = (seq_t*)malloc(sizeof(seq_t) * next_len);

        /* ---- Compute u(n,x) ---- */
        /* Unext[i] starts at 0. */
        for (size_t i = 0; i < next_len; ++i) Unext[i] = 0;

        /* u(n,x) = u(n-1,x) + x*v(n-1,x) + 1 */
        for (size_t i = 0; i < Ulen; ++i) {
            Unext[i]   += Uprev[i];   /* u(n-1,x)                         */
            Unext[i+1] += Vprev[i];   /* x * v(n-1,x)                      */
        }
        Unext[0] += 1;                /* constant term from "+ 1"           */

        /* ---- Compute v(n,x) ---- */
        /* Vnext[i] starts at 0. */
        for (size_t i = 0; i < next_len; ++i) Vnext[i] = 0;

        /* v(n,x) = (x+2)*u(n-1,x) + (x-1)*v(n-1,x) */
        for (size_t i = 0; i < Ulen; ++i) {
            /* (x+2)*u:  x*u + 2*u */
            Vnext[i+1] += Uprev[i];   /* x*u */
            Vnext[i]   += Uprev[i];   /* 2*u (by adding twice) */
            Vnext[i]   += Uprev[i];

            /* (x-1)*v:  x*v - v */
            Vnext[i+1] += Vprev[i];   /* x*v */
            Vnext[i]   -= Vprev[i];   /* -v */
        }

        /* ---- Append coefficients of v(n,x) to the flattened array ---- */
        flat = (seq_t*)realloc(flat, sizeof(seq_t) * (flat_len + next_len));
        for (size_t i = 0; i < next_len; ++i) {
            flat[flat_len + i] = Vnext[i];
        }
        flat_len += next_len;

        /* ---- Prepare for next iteration ---- */
        free(Uprev);   free(Vprev);
        Uprev = Unext; Vprev = Vnext;
        Ulen = Vlen = next_len;
        built_row = next_row;
    }

    /* Return the requested coefficient.  (Indices are 1‑based.) */
    return flat[n - 1];
}
