/*
 * A176957  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=4, k=-1 and l=-1.
 * https://oeis.org/A176957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4 For n>=1: a(n+1) = sum_{p=0}^{n} (a(p)*a(n-p)+k)
 *            + l with k=-1, l=-1, i.e. a(n+1) = sum_{p=0}^{n} a(p)*a(n-p) -
 *            (n+1) - 1 Thus for i>=2: a[i] = sum_{p=0}^{i-1} a[p]*a[i-1-p] -
 *            i - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;          /* cache of a(0)..a(current) */
static size_t cap = 0;           /* allocated size of A */
static idx_t max_computed = 0;   /* highest index filled in A */

static void ensure_capacity(idx_t need) {
    if ((size_t)need + 1 > cap) {
        size_t new_cap = (size_t)need + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* In an extension package this would handle out‑of‑memory,
               but for simplicity we abort. */
            exit(1);
        }
        A = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    if (max_computed < n) {
        ensure_capacity(n);
        /* initialize base cases if needed */
        if (max_computed == 0) {
            A[0] = (seq_t)1;
            if (n >= 1) A[1] = (seq_t)4;
            max_computed = 1;
        }
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t conv = 0;
            /* compute convolution sum a[p]*a[i-1-p] */
            for (idx_t p = 0; p <= i - 1; ++p) {
                conv += A[p] * A[i - 1 - p];
            }
            /* a[i] = conv - i - 1  (k = -1, l = -1) */
            A[i] = conv - (seq_t)i - (seq_t)1;
        }
        max_computed = n;
    }
    return A[n];
}
