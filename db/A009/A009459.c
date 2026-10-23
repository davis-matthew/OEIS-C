/*
 * A009459  Expansion of sin(log(1+x))/cos(x).
 * https://oeis.org/A009459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * coeff of x^n in sin(log(1+x))/cos(x)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The integer sequence a(n) is computed by the recurrence */
/* a(0)=0, a(1)=1 and for n≥2 */
/*   a(n) = p(n) + Σ_{k=1}^{⌊n/2⌋} (-1)^{k+1} * C(n,2k) * a(n-2k)   */
/* where p(n) = Im((1+x)^i) * n! = Im(c_n) and c_n satisfies */
/*   c_0 = 1,  c_{n+1} = c_n * (i – n)  with c_n = Re(c_n)+iIm(c_n)   */
/* The recurrence uses only integer arithmetic. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static state for memoised computation */
    static size_t capacity = 0;   /* size of allocated arrays */
    static size_t used = 0;       /* highest index + 1 that has been computed */
    static seq_t *p = NULL;       /* p[n] = Im(c_n) = n! * s_n */
    static seq_t *A = NULL;       /* A[n] = a(n)  (integer sequence) */
    static idx_t cur_n = 0;       /* index of the last computed c_n   */
    static seq_t cur_re = 1;      /* Re(c_cur_n)                      */
    static seq_t cur_im = 0;      /* Im(c_cur_n)                      */

    /* enlarge storage if necessary */
    if (used <= (size_t)n) {
        size_t need = (size_t)n + 1;
        if (capacity < need) {
            size_t new_cap = capacity ? capacity * 2 : 8;
            while (new_cap < need) new_cap <<= 1;
            seq_t *new_p = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
            seq_t *new_A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (!new_p || !new_A) _Exit(EXIT_FAILURE);
            p = new_p;
            A = new_A;
            capacity = new_cap;
        }

        /* initialise first element if we have never computed anything */
        if (used == 0) {
            cur_n  = 0;
            cur_re = 1;  /* c_0 = 1 */
            cur_im = 0;  /* Im(c_0) = 0 */
            p[0] = cur_im;          /* p_0 = 0 */
            A[0] = 0;               /* a(0) = 0 */
            used = 1;
        }

        /* compute values from the last computed index up to n */
        for (size_t i = used; i <= n; ++i) {
            /* update c_{i} from c_{i-1} using the recurrence */
            seq_t new_re = -(seq_t)cur_n * cur_re - cur_im;
            seq_t new_im = cur_re - (seq_t)cur_n * cur_im;
            cur_re = new_re;
            cur_im = new_im;
            cur_n  = (idx_t)i;                /* new index */

            p[i] = cur_im;                    /* p_i = Imag(c_i) */

            /* compute a(i) from the recurrence */
            if (i == 1) {
                A[1] = 1;
            } else if (i >= 2) {
                seq_t sum = p[i];
                size_t max_k = i / 2;
                seq_t comb = 1;  /* C(i,0) */

                for (size_t k = 1; k <= max_k; ++k) {
                    /* advance comb to C(i, 2k) using the 2‑step recurrence */
                    comb = comb * (seq_t)(i - 2 * k + 2);
                    comb = comb * (seq_t)(i - 2 * k + 1);
                    comb = comb / ( (seq_t)(2 * k) * (seq_t)(2 * k - 1) );

                    seq_t term = comb * A[i - 2 * k];
                    if (k & 1) {           /* k odd → sign +1 */
                        sum += term;
                    } else {               /* k even → sign –1 */
                        sum -= term;
                    }
                }
                A[i] = sum;
            }
        }
        used = (size_t)n + 1;
    }

    return A[n];
}
