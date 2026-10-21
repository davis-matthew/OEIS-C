/*
 * A083954  Least integer coefficients of A(x), where 1<=a(n)<=4, such that
 *          A(x)^(1/4) consists entirely of integer coefficients.
 * https://oeis.org/A083954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A083954
   The values a(n) (1 ≤ a(n) ≤ 4) are the least integer coefficients
   of a polynomial A(x) such that A(x)^(1/4) has only integer
   coefficients.  a(0)=1.  The following routine computes a(n)
   for any non‑negative n, memoising all previously computed values. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The build supplies seq_t (unsigned) and idx_t (uint64_t).  */
extern idx_t _floor_isqrt_u64(idx_t);  /* unused */

/* Internal dynamic arrays holding signed 64‑bit values to keep all
   intermediate computations free from overflow for n ≤ 10000.  */
static int64_t *b      = NULL;   /* b[i]  = coefficient of x^i in B(x)          */
static int64_t *pow2   = NULL;   /* coefficients of B(x)^2                     */
static int64_t *pow3   = NULL;   /* coefficients of B(x)^3                     */
static int64_t *pow4   = NULL;   /* coefficients of B(x)^4                     */
static seq_t   *a_seq  = NULL;   /* final a(n) values                          */
static size_t    cap     = 0;     /* current allocation size                   */
static size_t    cur_len = 0;     /* number of a_seq entries that are defined   */
static bool      init    = false; /* true after first initialisation            */

/* Ensure array capacity for degree n (0‑based).  All arrays are sized to
   at least 4*n+1 so that indices up to 4*n can be written safely.  */
static void ensure_capacity(idx_t n)
{
    size_t needed = 4 * (size_t)n + 1;
    if (needed <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    b     = (int64_t *)realloc(b,     new_cap * sizeof(int64_t));
    pow2  = (int64_t *)realloc(pow2,  new_cap * sizeof(int64_t));
    pow3  = (int64_t *)realloc(pow3,  new_cap * sizeof(int64_t));
    pow4  = (int64_t *)realloc(pow4,  new_cap * sizeof(int64_t));
    a_seq = (seq_t   *)realloc(a_seq, new_cap * sizeof(seq_t));
    for (size_t i = cap; i < new_cap; ++i) {
        b[i]     = 0;
        pow2[i]  = 0;
        pow3[i]  = 0;
        pow4[i]  = 0;
        a_seq[i] = 0;
    }
    cap = new_cap;
}

/* Compute or return the value of a(n).  */
seq_t a(idx_t n)
{
    /* initialise on first call (a(0)=1, b(0)=1). */
    if (!init) {
        ensure_capacity(0);
        b[0]    = 1;          /* b_0 = 1                     */
        pow2[0] = 1;          /* B^2       coefficient 0     */
        pow3[0] = 1;          /* B^3       coefficient 0     */
        pow4[0] = 1;          /* B^4       coefficient 0     */
        a_seq[0] = 1;         /* a_0 = 1                      */
        cur_len = 1;
        init = true;
    }

    /* extend memoised region up to n (if needed). */
    if ((idx_t)cur_len <= n) {
        for (idx_t idx = (idx_t)cur_len; idx <= n; ++idx) {
            /* make sure arrays large enough for this degree. */
            ensure_capacity(idx);

            /* current S(k) = coefficient of x^idx in B^4 built so far. */
            int64_t S = pow4[(size_t)idx];

            /* choose the least a in 1..4 with a ≡ S (mod 4). */
            int mod4 = (int)(S % 4);
            if (mod4 < 0) mod4 += 4;            /* handle negative S */
            seq_t a_val = (mod4 == 0) ? 4 : (seq_t)mod4;

            /* b_k = (a_k - S)/4.  The division is exact. */
            int64_t diff = (int64_t)a_val - S;
            int64_t b_val = diff / 4;           /* integer division */

            /* store results */
            a_seq[(size_t)idx] = a_val;
            b[(size_t)idx]     = b_val;

            /* pre‑compute powers of b_val needed for updates */
            int64_t b_pow2 = b_val * b_val;
            int64_t b_pow3 = b_pow2 * b_val;
            int64_t b_pow4 = b_pow3 * b_val;

            /* lengths of the previous polynomials */
            size_t len_pow3_old = 3 * ((size_t)idx - 1) + 1;
            size_t len_pow2_old = 2 * ((size_t)idx - 1) + 1;
            size_t len_pow1_old = (size_t)idx;           /* b[0..idx-1] */

            /* --- Update B^4 --------------------------------------- */
            /* add 4*b * B^3  (shift by idx) */
            int64_t scale1 = 4 * b_val;
            for (size_t j = 0; j < len_pow3_old; ++j)
                pow4[j + (size_t)idx] += scale1 * pow3[j];

            /* add 6*b^2 * B^2 (shift by 2*idx) */
            int64_t scale2 = 6 * b_pow2;
            for (size_t j = 0; j < len_pow2_old; ++j)
                pow4[j + 2 * (size_t)idx] += scale2 * pow2[j];

            /* add 4*b^3 * B (shift by 3*idx) */
            int64_t scale3 = 4 * b_pow3;
            for (size_t j = 0; j < len_pow1_old; ++j)
                pow4[j + 3 * (size_t)idx] += scale3 * b[j];

            /* add b^4 (shift by 4*idx) */
            pow4[4 * (size_t)idx] += b_pow4;

            /* --- Update B^3 --------------------------------------- */
            /* add 3*b * B^2 (shift by idx) */
            int64_t scale3_1 = 3 * b_val;
            for (size_t j = 0; j < len_pow2_old; ++j)
                pow3[j + (size_t)idx] += scale3_1 * pow2[j];

            /* add 3*b^2 * B (shift by 2*idx) */
            int64_t scale3_2 = 3 * b_pow2;
            for (size_t j = 0; j < len_pow1_old; ++j)
                pow3[j + 2 * (size_t)idx] += scale3_2 * b[j];

            /* add b^3 (shift by 3*idx) */
            pow3[3 * (size_t)idx] += b_pow3;

            /* --- Update B^2 --------------------------------------- */
            /* add 2*b * B (shift by idx) */
            int64_t scale2_1 = 2 * b_val;
            for (size_t j = 0; j < len_pow1_old; ++j)
                pow2[j + (size_t)idx] += scale2_1 * b[j];

            /* add b^2 (shift by 2*idx) */
            pow2[2 * (size_t)idx] += b_pow2;
        }
        cur_len = (size_t)n + 1;
    }

    return a_seq[(size_t)n];
}
