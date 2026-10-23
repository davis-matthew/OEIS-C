/*
 * A171094  a(n) is the string of integers S(n) concatenated together with no
 *          commas, where S(1)=1; for even n, S(n) is obtained from S(n-1) by
 *          inserting n after 1; and for odd n>1, S(n) = n,S(n-1).
 * https://oeis.org/A171094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: S(1)=1. For n≥2, if n is even, S(n) is obtained from S(n‑1)
   by inserting n after the first digit '1'; if n is odd, S(n)=n concatenated with S(n‑1).
   a(n) is the integer value represented by S(n). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static seq_t  *prefix_arr   = NULL;   /* digits before the first '1' */
static seq_t  *suffix_arr   = NULL;   /* digits after the first '1'  */
static seq_t  *value_arr    = NULL;   /* a(n) */
static size_t *lenP_arr     = NULL;   /* length of prefix    */
static size_t *lenS_arr     = NULL;   /* length of suffix    */
static idx_t   max_n        = 0;
static size_t  capacity     = 0;

static void extend_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity : 4;
        while (new_cap < needed) new_cap <<= 1;
        prefix_arr = (seq_t *)realloc(prefix_arr, new_cap * sizeof(seq_t));
        suffix_arr = (seq_t *)realloc(suffix_arr, new_cap * sizeof(seq_t));
        value_arr  = (seq_t *)realloc(value_arr,  new_cap * sizeof(seq_t));
        lenP_arr   = (size_t *)realloc(lenP_arr,  new_cap * sizeof(size_t));
        lenS_arr   = (size_t *)realloc(lenS_arr,  new_cap * sizeof(size_t));
        capacity = new_cap;
    }
}

static void build_upto(idx_t n)
{
    if (n <= max_n) return;
    extend_capacity(n);
    for (idx_t k = max_n + 1; k <= n; ++k) {
        if (k == 1) {
            /* S(1) = "1" */
            prefix_arr[1] = 0;
            lenP_arr[1]   = 0;
            suffix_arr[1] = 0;
            lenS_arr[1]   = 0;
            value_arr[1]  = 1;
        } else {
            idx_t km1 = k - 1;
            seq_t prev_prefix = prefix_arr[km1];
            size_t prev_lenP   = lenP_arr[km1];
            seq_t prev_suffix = suffix_arr[km1];
            size_t prev_lenS   = lenS_arr[km1];

            seq_t new_prefix, new_suffix;
            size_t new_lenP, new_lenS;

            int len_k = digit_len((seq_t)k);      /* digits in n */

            if ((k & 1) == 0) { /* even: insert after first '1' */
                seq_t pow_s     = pow10((idx_t)prev_lenS);               /* 10^lenS_prev */
                new_suffix     = seq_mul((seq_t)k, pow_s);              /* k * 10^lenS_prev */
                new_suffix     = new_suffix + prev_suffix;              /* + old suffix */
                new_lenS       = prev_lenS + len_k;
                new_prefix     = prev_prefix;
                new_lenP       = prev_lenP;
            } else { /* odd (k>1): prepend k */
                seq_t pow_p     = pow10((idx_t)prev_lenP);               /* 10^lenP_prev */
                new_prefix     = seq_mul((seq_t)k, pow_p);              /* k * 10^lenP_prev */
                new_prefix     = new_prefix + prev_prefix;              /* + old prefix */
                new_lenP       = prev_lenP + len_k;
                new_suffix     = prev_suffix;
                new_lenS       = prev_lenS;
            }

            prefix_arr[k] = new_prefix;
            lenP_arr[k]   = new_lenP;
            suffix_arr[k] = new_suffix;
            lenS_arr[k]   = new_lenS;

            seq_t mult10S = pow10((idx_t)(new_lenS + 1)); /* 10^(lenS+1) */
            seq_t multS   = pow10((idx_t)new_lenS);       /* 10^lenS       */
            seq_t val     = seq_mul(new_prefix, mult10S);
            val = val + multS;
            val = val + new_suffix;
            value_arr[k]  = val;
        }
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    build_upto(n);
    return value_arr[n];
}
