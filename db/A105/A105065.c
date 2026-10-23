/*
 * A105065  First entry of the vector v(n), where v(0) is the 2 by 2 column
 *          vector [0,1], v(n)=(M(n-1)^(n-1))v(n-1) and M(k) is the 2 x 2
 *          matrix [[0,1],[1,k]].
 * https://oeis.org/A105065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first entry of v(n), where v(0) = [0;1] and v(n) =
 *            (M(n-1)^(n-1)) * v(n-1), with M(k) = [[0,1],[1,k]]. This
 *            function memoizes all computed values so that calls in any order
 *            are efficient
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoized storage */
    static seq_t *a_vals = NULL;   /* first components a(n) */
    static seq_t *b_vals = NULL;   /* second components b(n) */
    static idx_t cap   = 0;        /* allocated capacity */
    static idx_t len   = 0;        /* number of elements stored (len>=1 after init) */

    if (cap == 0) {                /* first call: initialise for n=0 */
        cap = 1;
        a_vals = (seq_t *)malloc(cap * sizeof(seq_t));
        b_vals = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!a_vals || !b_vals) return (seq_t)0; /* allocation failed */
        a_vals[0] = (seq_t)0;
        b_vals[0] = (seq_t)1;
        len = 1;
    }

    /* compute values up to requested index n */
    while (len <= n) {
        /* index of the new element we are computing */
        idx_t cur = len;              /* cur == n to be computed */

        /* build the matrix M(cur-1) = [[0,1],[1,cur-1]] */
        seq_t M[2][2];
        M[0][0] = (seq_t)0;
        M[0][1] = (seq_t)1;
        M[1][0] = (seq_t)1;
        M[1][1] = (seq_t)(cur - 1);    /* k = cur-1 */

        /* compute P = M^(cur-1) with binary exponentiation */
        seq_t P[2][2];                 /* will hold M^exp */
        P[0][0] = (seq_t)1; P[0][1] = (seq_t)0;
        P[1][0] = (seq_t)0; P[1][1] = (seq_t)1;

        idx_t exp = cur - 1;
        seq_t base[2][2];
        base[0][0] = M[0][0]; base[0][1] = M[0][1];
        base[1][0] = M[1][0]; base[1][1] = M[1][1];

        while (exp > 0) {
            if (exp & 1) {
                /* P = P * base */
                seq_t tmp[2][2];
                tmp[0][0] = P[0][0] * base[0][0] + P[0][1] * base[1][0];
                tmp[0][1] = P[0][0] * base[0][1] + P[0][1] * base[1][1];
                tmp[1][0] = P[1][0] * base[0][0] + P[1][1] * base[1][0];
                tmp[1][1] = P[1][0] * base[0][1] + P[1][1] * base[1][1];
                P[0][0] = tmp[0][0]; P[0][1] = tmp[0][1];
                P[1][0] = tmp[1][0]; P[1][1] = tmp[1][1];
            }

            /* base = base * base */
            seq_t tmp2[2][2];
            tmp2[0][0] = base[0][0] * base[0][0] + base[0][1] * base[1][0];
            tmp2[0][1] = base[0][0] * base[0][1] + base[0][1] * base[1][1];
            tmp2[1][0] = base[1][0] * base[0][0] + base[1][1] * base[1][0];
            tmp2[1][1] = base[1][0] * base[0][1] + base[1][1] * base[1][1];
            base[0][0] = tmp2[0][0]; base[0][1] = tmp2[0][1];
            base[1][0] = tmp2[1][0]; base[1][1] = tmp2[1][1];

            exp >>= 1;
        }

        /* multiply P by the previous vector (a_{cur-1}, b_{cur-1}) */
        seq_t a_prev = a_vals[cur - 1];
        seq_t b_prev = b_vals[cur - 1];
        seq_t new_a = P[0][0] * a_prev + P[0][1] * b_prev;
        seq_t new_b = P[1][0] * a_prev + P[1][1] * b_prev;

        /* grow storage if needed */
        if (len == cap) {
            idx_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq_t *a_new = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            seq_t *b_new = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
            if (!a_new || !b_new) return (seq_t)0; /* allocation failed */
            a_vals = a_new;
            b_vals = b_new;
            cap = new_cap;
        }

        a_vals[len] = new_a;
        b_vals[len] = new_b;
        len++;
    }

    return a_vals[n];
}
