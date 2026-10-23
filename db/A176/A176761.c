/*
 * A176761  Partial sums of A030001, starting at n=1.
 * https://oeis.org/A176761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digit_len, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A176761: the partial sums of the minimal powers of 2 that contain n in decimal.
   a(n) = sum_{k=1..n} min{2^e} such that the decimal representation of k appears as a
   substring of 2^e.  The function memoizes intermediate results so that calls in
   any order up to n = 10000 remain fast.  */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"

/* cached powers of two */
static seq_t *powers = NULL;
static size_t powers_len = 0;
static size_t powers_cap = 0;

/* cached cumulative sums */
static seq_t *sums = NULL;
static size_t sums_len = 0;
static size_t sums_cap = 0;
static idx_t last_computed = 0;

/* make powers[0..e] available, computing as needed */
static void ensure_power(idx_t e)
{
    if ((idx_t)powers_len > e) return;
    size_t new_cap = powers_cap ? powers_cap : 1;
    while (new_cap <= (size_t)e + 1) new_cap <<= 1;
    if (new_cap != powers_cap) {
        powers = (seq_t *)realloc(powers, new_cap * sizeof(seq_t));
        powers_cap = new_cap;
    }
    for (size_t i = powers_len; i <= (size_t)e; ++i) {
        if (i == 0) powers[0] = (seq_t)1;
        else powers[i] = powers[i-1] * (seq_t)2;
    }
    powers_len = e + 1;
}

/* make sums[0..i] available, allocating storage as needed */
static void ensure_sum(idx_t i)
{
    if ((idx_t)sums_len > i) return;
    size_t new_cap = sums_cap ? sums_cap : 1;
    while (new_cap <= (size_t)i + 1) new_cap <<= 1;
    if (new_cap != sums_cap) {
        sums = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
        sums_cap = new_cap;
    }
    if (sums_len == 0) {          /* initialise the zero‐term */
        sums[0] = (seq_t)0;
    }
    sums_len = i + 1;
}

/* return the smallest power of 2 whose decimal representation contains i */
static seq_t min_power_with_i(idx_t i)
{
    /* convert i to a decimal string once per call */
    char target[32];
    int target_len = snprintf(target, sizeof(target), "%llu", (unsigned long long)i);
    (void)target_len;                    /* silence unused warning */

    static char *buf = NULL;
    static size_t buf_cap = 0;
    static size_t buf_len = 0;            /* current digit length of the power */

    for (idx_t e = 0; ; ++e) {
        ensure_power(e);
        seq_t p = powers[e];

        size_t digits = (size_t)digit_len(p);
        if (digits != buf_len) {
            buf_len = digits;
            size_t needed = digits + 1;   /* +1 for nul terminator */
            if (needed > buf_cap) {
                char *nb = (char *)realloc(buf, needed);
                if (!nb) return (seq_t)0; /* unlikely */
                buf = nb;
                buf_cap = needed;
            }
        }

        seq_to_str(p, buf, &buf_len);
        buf[buf_len] = '\0';

        if (strstr(buf, target) != NULL)
            return p;
    }
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for offset */

    if ((idx_t)sums_len <= n) {
        ensure_sum(n);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = min_power_with_i(i);
            sums[i] = sums[i-1] + val;
        }
        last_computed = n;
    }
    return sums[n];
}
