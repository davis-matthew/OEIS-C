/*
 * A084806  Least positive integers, all distinct, that satisfy Sum_{n>0}
 *          1/a(n)^z = 0, where z=1/2 + I.
 * https://oeis.org/A084806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate A084806 by incremental search ensuring the
   magnitude of the partial sum of 1/a(k)^z (z = 1/2 + i) is strictly
   decreasing.  The sequence is stored in a static cache to allow
   non‑monotonic calls.  The implementation works for any compiler
   supporting seq_t as an unsigned integer type or an arbitrary‑precision
   integer class. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* compute a(n) – the nth term of OEIS A084806 */
seq_t a(idx_t n)
{
    /* static cache: sequence values (1‑based) */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;   /* number of terms currently stored */
    static idx_t seq_cap = 0;   /* allocated capacity for seq */

    /* array of flags: used[i] == 1 if integer i already used */
    static unsigned char *used = NULL;
    static idx_t used_cap = 0;

    /* current partial sum of 1/a(k)^z  (z = 1/2 + i) in double precision */
    static double sum_re = 0.0; /* real part  */
    static double sum_im = 0.0; /* imaginary part */

    /* keep track of the last integer for which we started the search */
    static idx_t last_cand = 0;

    /* trivial case: n == 0 not part of the definition – return 0 safely */
    if (n == 0)
        return (seq_t)0;

    /* initialise on first call */
    if (seq_len == 0) {
        /* initial capacity for the sequence */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        seq[0] = (seq_t)1;        /* a(1) = 1 */
        seq_len = 1;

        /* initial capacity for the used array */
        used_cap = 16;
        used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        used[1] = 1;               /* mark 1 as used */

        sum_re = 1.0;              /* 1 / 1^(0.5+i)  →  real part */
        sum_im = 0.0;              /* imaginary part is zero */

        last_cand = 1;             /* last candidate checked = 1 */
    }

    /* generate terms until we have at least n terms */
    while (seq_len < n) {

        /* start searching from the next integer after the previous one */
        idx_t cand = last_cand + 1;

        /* ensure the 'used' array can hold this index */
        while (cand >= used_cap) {
            idx_t new_cap = used_cap ? used_cap * 2 : 16;
            while (new_cap <= cand)
                new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
            if (!tmp)
                exit(1);
            memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
            used = tmp;
            used_cap = new_cap;
        }

        /* search for the smallest admissible candidate */
        for (;;) {
            if (!used[cand]) {
                /* compute 1 / cand^z   where z = 0.5 + i                     */
                double v    = 1.0 / sqrt((double)cand); /* |cand|^{-1/2}     */
                double lg   = log((double)cand);        /* log(cand)          */
                double cr   =  v * cos(lg);             /* real part of 1/cand^z */
                double ci   = -v * sin(lg);             /* imaginary part          */
                double new_re = sum_re + cr;
                double new_im = sum_im + ci;
                double mag2_new = new_re * new_re + new_im * new_im;
                double mag2_old = sum_re * sum_re + sum_im * sum_im;

                if (mag2_new < mag2_old) {
                    /* accept this candidate */
                    if (seq_len == seq_cap) {
                        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                        if (!tmp)
                            exit(1);
                        seq = tmp;
                        seq_cap = new_cap;
                    }
                    seq[seq_len] = (seq_t)cand;   /* convert to seq_t */
                    seq_len++;

                    used[cand] = 1;
                    sum_re = new_re;
                    sum_im = new_im;
                    last_cand = cand;             /* update for next search */
                    break;                          /* move on to next term */
                }
            }
            /* otherwise try the next integer */
            cand++;

            /* ensure the 'used' array can hold this index */
            while (cand >= used_cap) {
                idx_t new_cap = used_cap ? used_cap * 2 : 16;
                while (new_cap <= cand)
                    new_cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
                if (!tmp)
                    exit(1);
                memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
                used = tmp;
                used_cap = new_cap;
            }
        }
    }

    return seq[n - 1];
}
