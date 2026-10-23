/*
 * A115106  Let M0 = {{2, 2, 0}, {1, 0, 1}, {0, 2, 2}}; M1 = {{1, 0, 1}, {2,
 *          2, 0}, {0, 2, 2}}; M2 = {{2, 2, 0}, {0, 2, 2}, {1, 0, 1}}; M[n_]
 *          := M[n] = If[Mod[v[n][[1]], 3] == 0, M1, If[Mod[v[n][[2]], 3] ==
 *          0, M0, M2]] v[0] = {1, 1, 1}; M[0] = {{2, 2, 0}, {1, 0, 1}, {0, 2,
 *          2}}; v[n_] := v[n] = M[n - 1].v[n -...
 * https://oeis.org/A115106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mul_M0, mul_M1, mul_M2, seq_realloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115106: a(n) is the first component of the vector v[n] obtained by
   starting from v[0]={1,1,1} and repeatedly multiplying by a 3×3 matrix
   that is chosen from the following three,
   depending on the remainder of the preceding vector modulo 3.
   M0 = {{2,2,0},{1,0,1},{0,2,2}}, M1 = {{1,0,1},{2,2,0},{0,2,2}}, M2 = {{2,2,0},{0,2,2},{1,0,1}},
   where M0 is used for n=0 and after that
   M1 is used if v[n-1][0]≡0 (mod3),
   otherwise M0 is used if v[n-1][1]≡0 (mod3),
   otherwise M2 is used.  The sequence begins
   1,4,12,20,136,392,…                           */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_M0.h"
#include "../utils/mul_M1.h"
#include "../utils/mul_M2.h"
#include "../utils/seq_realloc_array.h"

#ifdef __cplusplus

#else

#endif

/* Cached values of the sequence. */
static seq_t *seq_a = NULL;   /* array of a(i) */
static idx_t seq_cap = 0;     /* current allocated capacity (elements) */
static idx_t seq_max = 0;     /* highest index computed (≥0) */
static bool seq_init = false;/* true after first call */

/* The current vector v[seq_max]. */
static seq_t vec_cur[3] = {1, 1, 1};

seq_t a(idx_t n)
{
    /* Initialise once. */
    if (!seq_init) {
        seq_init = true;
        seq_cap = 1;
        seq_a   = seq_realloc_array(NULL, 0, seq_cap);
        seq_a[0] = (seq_t)1;       /* a(0) */
        seq_max  = 0;
    }

    /* Resize array if necessary. */
    if (n + 1 > seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_a = seq_realloc_array(seq_a, seq_cap, new_cap);
        seq_cap = new_cap;
    }

    /* Extend computation up to n. */
    while (seq_max < n) {
        seq_t v_new[3];
        if (seq_max == 0) {                /* use M0 for the first step */
            mul_M0(v_new, vec_cur);
        } else if ((vec_cur[0] % 3) == 0) {  /* M1 if v[prev][0]≡0 (mod3) */
            mul_M1(v_new, vec_cur);
        } else if ((vec_cur[1] % 3) == 0) {  /* M0 if v[prev][1]≡0 (mod3) */
            mul_M0(v_new, vec_cur);
        } else {                           /* otherwise M2 */
            mul_M2(v_new, vec_cur);
        }

        seq_max++;
        seq_a[seq_max] = v_new[0];
        vec_cur[0] = v_new[0];
        vec_cur[1] = v_new[1];
        vec_cur[2] = v_new[2];
    }

    return seq_a[n];
}
