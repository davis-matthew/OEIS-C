/*
 * A055047  Numbers of the form 9^i*(3*j+1).
 * https://oeis.org/A055047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest positive integer of the form
 *            9^i·(3·j+1) with i,j ≥ 0. The function obtains a(n) by
 *            iteratively merging i = 0…20 arithmetic progressions 9^i·(3j+1)
 *            and selecting the next smallest value
 * Range:     u8 n <= 10, u16 n <= 2705, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t pow9[21];
static seq_t step[21];
static int pow9_initialized = 0;

/* Initialise the tables of 9^i and the associated steps 9^i*3.
   We only need i up to 20 because 9^21 would overflow a 64‑bit
   integer and no term needed for the typical limits of the tests
   will resort to that index. */
static void init_pow9_step(void)
{
    if (pow9_initialized) return;
    pow9_initialized = 1;
    pow9[0] = (seq_t)1;
    for (int i = 1; i < 21; ++i) {
        pow9[i] = pow9[i-1] * (seq_t)9;
        step[i]  = pow9[i] * (seq_t)3;   /* increment for each j */
    }
    /* For i = 0, step is simply 3, which will be recomputed on the fly. */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, undefined for n==0 */

    init_pow9_step();

    /* Local tables holding the current value and current j for each i.
       Both are small (21 elements), so we recompute them for every
       call; the cost is negligible. */
    seq_t cur_val[21];
    idx_t  cur_j[21];

    for (int i = 0; i < 21; ++i) {
        cur_j[i]  = (idx_t)0;
        cur_val[i] = pow9[i] * ((seq_t)3 * cur_j[i] + (seq_t)1); /* = pow9[i] */
    }

    seq_t result = 0;

    for (idx_t count = (idx_t)1; count <= n; ++count) {
        /* Find the minimal current value among all progressions. */
        seq_t min_val = cur_val[0];
        int   min_i   = 0;
        for (int i = 1; i < 21; ++i) {
            if (cur_val[i] < min_val) {
                min_val = cur_val[i];
                min_i   = i;
            }
        }

        result = min_val;                    /* this is the n‑th term */

        /* Advance the progression that supplied the minimal value. */
        ++cur_j[min_i];
        /* For i = 0 the step is simply 3; for i >= 1 we pre‑computed it. */
        if (min_i == 0) {
            cur_val[min_i] += (seq_t)3;      /* Next: 9^0*(3(j+1)+1) */
        } else {
            cur_val[min_i] += step[min_i];
        }
    }

    return result;
}
