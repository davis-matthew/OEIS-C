/*
 * A000739  Expansion of Product_{k>=1} (1 - x^k)^16.
 * https://oeis.org/A000739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [x^n] Product_{k>=1} (1 - x^k)^16, computed by
 *            multiplying 1 by Euler's pentagonal series Product_{k>=1} (1 -
 *            x^k) = Sum_{k} (-1)^k x^(k(3k-1)/2) 16 times
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 261, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"

static seq_t *cache = NULL;
static long long cache_n = -1;

static void build(long long N) {
    seq_t *arr = zalloc_arr((idx_t)(N + 1));
    long long *ex = (long long *)malloc(sizeof(long long) * 2 * (size_t)(N + 2));
    int *sg = (int *)malloc(sizeof(int) * 2 * (size_t)(N + 2));
    long long m = 0, k, i, j;
    int r;
    for (k = 1;; k++) {
        long long e1 = k * (3 * k - 1) / 2, e2 = k * (3 * k + 1) / 2;
        int s = (k & 1) ? -1 : 1;
        if (e1 > N) break;
        ex[m] = e1; sg[m] = s; m++;
        if (e2 <= N) { ex[m] = e2; sg[m] = s; m++; }
    }
    arr[0] = 1;
    for (r = 0; r < 16; r++) {
        for (i = N; i >= 0; i--) {
            seq_t s = arr[i];
            for (j = 0; j < m && ex[j] <= i; j++) {
                if (sg[j] > 0) s += arr[i - ex[j]];
                else s -= arr[i - ex[j]];
            }
            arr[i] = s;
        }
    }
    free(ex);
    free(sg);
    cache = arr;
    cache_n = N;
}

seq_t a(idx_t n) {
    if ((long long)n > cache_n) {
        long long N = (long long)n;
        if (N < 2 * cache_n) N = 2 * cache_n;
        if (N < 64) N = 64;
        build(N);
    }
    return cache[n];
}
