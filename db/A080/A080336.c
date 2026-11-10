/*
 * A080336  Partial sums of A007001.
 * https://oeis.org/A080336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A007001(k), a(0) = 0; A007001 is the fixed
 *            point of the morphism k -> 1,2,...,k+1 starting from 1
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned int *A007001_s = NULL;
static idx_t A007001_len = 0;
static idx_t A007001_cap = 0;
static idx_t A007001_next = 1;

static unsigned int A007001_a(idx_t n) {
    if (A007001_s == NULL) {
        A007001_cap = 64;
        A007001_s = (unsigned int *)malloc((size_t)(A007001_cap + 1) * sizeof(unsigned int));
        A007001_s[1] = 1;
        A007001_len = 0;
        A007001_next = 1;
    }
    while (A007001_len < n) {
        unsigned int k = A007001_s[A007001_next];
        idx_t need = A007001_len + (idx_t)k + 1;
        if (need > A007001_cap) {
            idx_t nc = A007001_cap * 2;
            while (nc < need) nc *= 2;
            A007001_s = (unsigned int *)realloc(A007001_s, (size_t)(nc + 1) * sizeof(unsigned int));
            A007001_cap = nc;
        }
        for (unsigned int j = 1; j <= k + 1; j++) {
            A007001_s[++A007001_len] = j;
        }
        A007001_next++;
    }
    return A007001_s[n];
}

seq_t a(idx_t n) {
    static idx_t last_n = 0;
    static seq_t last_v = 0;
    idx_t i = 0;
    seq_t sum = 0;
    if (n >= last_n) {
        i = last_n;
        sum = last_v;
    }
    for (i = i + 1; i <= n; i++) {
        sum += (seq_t)A007001_a(i);
    }
    if (n >= last_n) {
        last_n = n;
        last_v = sum;
    }
    return sum;
}
