/*
 * A098304  Member r=19 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - a(n-2) + 2, a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* The sequence array grows on demand and is memoized across calls. */
static seq_t *seq_arr = NULL;   /* array holding computed terms */
static idx_t    seq_cap = 0;    /* current capacity (elements) */
static idx_t    seq_len = 0;    /* number of terms already computed */

/* Ensure that seq_arr has capacity at least needed */
static void seq_ensure(idx_t needed)
{
    if (needed < seq_cap) return;             /* already enough */
    /* Grow capacity: double until reaching needed+1 */
    idx_t new_cap = seq_cap ? seq_cap << 1 : 2;
    while (new_cap < needed + 1) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return; /* Out of memory: caller may handle */
    if (seq_arr) {
        memcpy(new_arr, seq_arr, seq_len * sizeof(seq_t));
        free(seq_arr);
    }
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* Memoized computation of the sequence */
seq_t a(idx_t n)
{
    if (n < seq_len) return seq_arr[n];     /* already computed */
    /* Ensure capacity for up to n */
    seq_ensure(n);
    /* Compute missing terms */
    if (seq_len == 0) {
        seq_arr[0] = (seq_t)0;
        seq_len = 1;
    }
    if (seq_len == 1 && n >= 1) {
        seq_arr[1] = (seq_t)1;
        seq_len = 2;
    }
    for (idx_t i = seq_len; i <= n; ++i) {
        seq_t t = seq_arr[i-1] * (seq_t)17;   /* 17*a(i-1) */
        t -= seq_arr[i-2];                   /* - a(i-2) */
        t += (seq_t)2;                       /* + 2 */
        seq_arr[i] = t;
    }
    seq_len = n + 1;
    return seq_arr[n];
}
