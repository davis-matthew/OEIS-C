/*
 * A221522  Number of 0..6 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +14*a(n-2) +29*a(n-3) +28*a(n-4) +a(n-5)
 *            +27*a(n-6) +8*a(n-7) +2*a(n-8); Initial values (n = 1..8):
 *            a(1)=0, a(2)=30, a(3)=130, a(4)=900, a(5)=5368, a(6)=33380,
 *            a(7)=205080, a(8)=1264378
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t brute_count(idx_t length)
{
    idx_t total = 1;
    for (idx_t i = 0; i < length; ++i) total *= 7;      /* 7^length */
    seq_t cnt = 0;

    int digits[8];   /* maximum length needed is 8 for initial terms */
    for (idx_t num = 0; num < total; ++num) {
        idx_t t = num;
        for (idx_t k = 0; k < length; ++k) {
            digits[k] = t % 7;
            t /= 7;
        }
        bool ok = true;
        for (idx_t k = 0; k < length; ++k) {
            bool neighbor = false;
            if (k > 0 && abs(digits[k] - digits[k-1]) >= 2) neighbor = true;
            if (k + 1 < length && abs(digits[k] - digits[k+1]) >= 2) neighbor = true;
            if (!neighbor) { ok = false; break; }
        }
        if (ok) ++cnt;
    }
    return cnt;
}

static idx_t known = 0;          /* highest index already computed */
static idx_t capacity = 0;       /* allocated capacity of arr */
static seq_t *arr = NULL;        /* array of computed values (1‑based) */

static void init_initial_terms(void)
{
    /* allocate space for indices 0..8 (index 0 unused) */
    #ifdef __cplusplus
    arr = new seq_t[9];
    #else
    arr = (seq_t*)malloc(sizeof(seq_t) * 9);
    #endif
    capacity = 9;
    known = 8;
    arr[0] = (seq_t)0;
    for (idx_t n = 1; n <= 8; ++n)
        arr[n] = brute_count(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but graceful */
    if (arr == NULL) init_initial_terms();

    if (n <= known) return arr[n];

    /* grow the array if necessary */
    if (n >= capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *new_arr = NULL;
    #ifdef __cplusplus
        new_arr = new seq_t[new_cap];
    #else
        new_arr = (seq_t*)malloc(sizeof(seq_t) * new_cap);
    #endif
        if (!new_arr) return (seq_t)0;        /* allocation failure */
        for (idx_t i = 0; i < capacity; ++i) new_arr[i] = arr[i];
    #ifdef __cplusplus
        delete[] arr;
    #else
        free(arr);
    #endif
        arr = new_arr;
        capacity = new_cap;
    }

    /* compute the recurrence for missing terms */
    for (idx_t i = known + 1; i <= n; ++i) {
        seq_t val = (seq_t)3 * arr[i-1] +
                    (seq_t)14 * arr[i-2] +
                    (seq_t)29 * arr[i-3] +
                    (seq_t)28 * arr[i-4] +
                    arr[i-5] +
                    (seq_t)27 * arr[i-6] +
                    (seq_t)8 * arr[i-7] +
                    (seq_t)2 * arr[i-8];
        arr[i] = val;
    }
    known = n;
    return arr[n];
}
