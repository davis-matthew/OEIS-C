/*
 * A047806  Duplicate of A008696.
 * https://oeis.org/A047806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 1 For n >= 1: Let s3(n) = σ_3(n) = sum_{d|n} d^3 s5(n) =
 *            σ_5(n) = sum_{d|n} d^5 Let c2_3(n) = sum_{a+b=n} s3(a)·s3(b)
 *            c3_3(n) = sum_{a+b+c=n} s3(a)·s3(b)·s3(c) c2_5(n) = sum_{a+b=n}
 *            s5(a)·s5(b) Then A4 = 3·240·s3 + 3·240^2·c2_3 + 240^3·c3_3 A6 =
 *            -2·504·s5 + 504^2·c2_5 a(n) = (13·A4 + 5·A6)...
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for computed tables */
static seq_t *s3          = NULL;
static seq_t *s5          = NULL;
static seq_t *c2_3        = NULL;
static seq_t *c3_3        = NULL;
static seq_t *c2_5        = NULL;
static seq_t *a_vals      = NULL;
static size_t cached_cap  = 0;   /* number of elements allocated */

/* helper to reallocate arrays up to desired capacity */
static void ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)(n + 1);
    if (new_cap <= cached_cap) return;   /* already big enough */

    /* free old memory if any */
    if (s3)    free(s3);
    if (s5)    free(s5);
    if (c2_3)  free(c2_3);
    if (c3_3)  free(c3_3);
    if (c2_5)  free(c2_5);
    if (a_vals) free(a_vals);

    /* allocate new arrays, zero-initialised */
    s3   = (seq_t*)calloc(new_cap, sizeof(seq_t));
    s5   = (seq_t*)calloc(new_cap, sizeof(seq_t));
    c2_3 = (seq_t*)calloc(new_cap, sizeof(seq_t));
    c3_3 = (seq_t*)calloc(new_cap, sizeof(seq_t));
    c2_5 = (seq_t*)calloc(new_cap, sizeof(seq_t));
    a_vals = (seq_t*)calloc(new_cap, sizeof(seq_t));

    /* 1. divisor sums σ_3 and σ_5 */
    for (size_t i = 1; i < new_cap; ++i) {
        /* i^3 and i^5 as seq_t; multiplications performed in __int128 */
        seq_t i3 = (seq_t) i * (seq_t)i * (seq_t)i;
        seq_t i5 = i3 * (seq_t)i * (seq_t)i;
        for (size_t j = i; j < new_cap; j += i) {
            __int128 t = ( __int128 ) s3[j] + ( __int128 ) i3;
            s3[j] = (seq_t)t;
            t = ( __int128 ) s5[j] + ( __int128 ) i5;
            s5[j] = (seq_t)t;
        }
    }

    /* 2. convolution σ_3 * σ_3 (ordered pairs) */
    for (size_t a = 1; a < new_cap; ++a) {
        for (size_t b = 1; b < new_cap - a; ++b) {
            __int128 prod = ( __int128 ) s3[a] * ( __int128 ) s3[b];
            __int128 cur  = ( __int128 ) c2_3[a + b] + prod;
            c2_3[a + b] = (seq_t)cur;
        }
    }

    /* 3. convolution of σ_3 * σ_3 * σ_3 (ordered triples) */
    for (size_t n = 2; n < new_cap; ++n) {  /* n=0,1 have 0 */
        __int128 sum = 0;
        for (size_t a = 1; a < n; ++a) {
            sum += ( __int128 ) c2_3[a] * ( __int128 ) s3[n - a];
        }
        c3_3[n] = (seq_t)sum;
    }

    /* 4. convolution σ_5 * σ_5 (ordered pairs) */
    for (size_t a = 1; a < new_cap; ++a) {
        for (size_t b = 1; b < new_cap - a; ++b) {
            __int128 prod = ( __int128 ) s5[a] * ( __int128 ) s5[b];
            __int128 cur  = ( __int128 ) c2_5[a + b] + prod;
            c2_5[a + b] = (seq_t)cur;
        }
    }

    /* 5. compute a(n) for all n < new_cap */
    a_vals[0] = (seq_t)1;   /* a(0)=1 */
    for (size_t n = 1; n < new_cap; ++n) {
        /* A4 = 3*240*s3 + 3*240^2*c2_3 + 240^3*c3_3 */
        __int128 t1 = ( __int128 ) s3[n]           * 720;          /* 3*240 */
        __int128 t2 = ( __int128 ) c2_3[n]         * 172800;        /* 3*240^2 */
        __int128 t3 = ( __int128 ) c3_3[n]         * 13824000;      /* 240^3 */
        __int128 A4 = t1 + t2 + t3;

        /* A6 = -2*504*s5 + 504^2*c2_5 */
        __int128 u1 = ( __int128 ) s5[n]           * -1008;         /* -2*504 */
        __int128 u2 = ( __int128 ) c2_5[n]         * 254016;        /* 504^2 */
        __int128 A6 = u1 + u2;

        /* a[n] = (13*A4 + 5*A6) / 18 */
        __int128 num = ( __int128 ) 13 * A4 + ( __int128 ) 5 * A6;
        a_vals[n] = (seq_t)(num / 18);
    }

    cached_cap = new_cap;
}

/* public interface required by the platform */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_vals[(size_t)n];
}
