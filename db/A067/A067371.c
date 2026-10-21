/*
 * A067371  Arithmetic derivatives of 3-smooth numbers.
 * https://oeis.org/A067371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of the nth 3‑smooth number N = 2^i
 *            · 3^j, computed as i·2^(i‑1)·3^j + j·2^i·3^(j‑1)
 * Range:     u8 n <= 19, u16 n <= 69, u32 n <= 279, u64 n <= 1153,
 *            u128 n <= 4808, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

/* Static state for generating 3‑smooth numbers */
static seq_t   *smooth_val = NULL;   /* power 2^i · 3^j */
static idx_t   *smooth_i   = NULL;   /* exponent of 2 */
static idx_t   *smooth_j   = NULL;   /* exponent of 3 */
static size_t   size       = 0;      /* number of generated terms */
static size_t   capacity   = 0;      /* allocated capacity */
static idx_t    ptr2       = 0;      /* next index to multiply by 2 */
static idx_t    ptr3       = 0;      /* next index to multiply by 3 */

/* Ensure the global tables contain at least n terms (1‑based). */
static void
ensure_terms(idx_t n)
{
    if (smooth_val == NULL) {
        /* initial allocation */
        capacity = 128;
        smooth_val = (seq_t*)malloc(capacity * sizeof(seq_t));
        smooth_i   = (idx_t*)  malloc(capacity * sizeof(idx_t));
        smooth_j   = (idx_t*)  malloc(capacity * sizeof(idx_t));

        /* N = 1  →  i = j = 0 */
        smooth_val[0] = (seq_t)1;
        smooth_i[0]   = 0;
        smooth_j[0]   = 0;
        size          = 1;
    }

    while ((size_t)size < (size_t)n) {
        /* candidates: next mult. by 2 or by 3 */
        seq_t val2 = smooth_val[ptr2] * (seq_t)2;
        seq_t val3 = smooth_val[ptr3] * (seq_t)3;

        seq_t next_val;
        idx_t  next_i, next_j;

        if (val2 < val3) {
            next_val = val2;
            next_i   = smooth_i[ptr2] + 1;
            next_j   = smooth_j[ptr2];
            ++ptr2;
        } else if (val2 > val3) {
            next_val = val3;
            next_i   = smooth_i[ptr3];
            next_j   = smooth_j[ptr3] + 1;
            ++ptr3;
        } else {                      /* equal – one number, advance both */
            next_val = val2;
            next_i   = smooth_i[ptr2] + 1;
            next_j   = smooth_j[ptr2];
            ++ptr2;
            ++ptr3;
        }

        /* grow storage if necessary */
        if (size == capacity) {
            capacity <<= 1;
            smooth_val = (seq_t*)realloc(smooth_val,
                                         capacity * sizeof(seq_t));
            smooth_i   = (idx_t*)realloc(smooth_i,
                                         capacity * sizeof(idx_t));
            smooth_j   = (idx_t*)realloc(smooth_j,
                                         capacity * sizeof(idx_t));
        }

        smooth_val[size] = next_val;
        smooth_i[size]   = next_i;
        smooth_j[size]   = next_j;
        ++size;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)           /* undefined, but return 0 for safety */
        return (seq_t)0;

    ensure_terms(n);

    seq_t  val = smooth_val[n - 1];
    idx_t  i   = smooth_i[n - 1];
    idx_t  j   = smooth_j[n - 1];

    /* arithmetic derivative:  i*2^(i-1)*3^j  +  j*2^i*3^(j-1) */
    seq_t result = (seq_t)0;

    if (i > 0) {
        seq_t pow2_i_minus1 = seq_pow((seq_t)2, i - 1);
        seq_t pow3_j        = seq_pow((seq_t)3, j);
        result += (seq_t)i * pow2_i_minus1 * pow3_j;
    }

    if (j > 0) {
        seq_t pow2_i        = seq_pow((seq_t)2, i);
        seq_t pow3_j_minus1 = seq_pow((seq_t)3, j - 1);
        result += (seq_t)j * pow2_i * pow3_j_minus1;
    }

    return result;
}
