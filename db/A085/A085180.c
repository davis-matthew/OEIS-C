/*
 * A085180  Array A(x,y) giving the position of the y-th x in A007001 listed
 *          by rising antidiagonals.
 * https://oeis.org/A085180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(x,y) = start(i) + x - 1, where i is the y-th block of A007001
 *            (block j = 1..s(j)+1) with s(i) >= x-1; read by antidiagonals
 *            A(d,1),A(d-1,2),...,A(1,d)
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned int *A085180_s = NULL;
static unsigned long long A085180_len = 0, A085180_cap = 0, A085180_next = 1;

static void A085180_ensure(unsigned long long n) {
    if (A085180_s == NULL) {
        A085180_cap = 64;
        A085180_s = (unsigned int *)malloc((size_t)(A085180_cap + 1) * sizeof(unsigned int));
        A085180_s[1] = 1;
        A085180_len = 0;
        A085180_next = 1;
    }
    while (A085180_len < n) {
        unsigned int k = A085180_s[A085180_next];
        unsigned long long need = A085180_len + k + 1;
        if (need > A085180_cap) {
            unsigned long long nc = A085180_cap * 2;
            while (nc < need) nc *= 2;
            A085180_s = (unsigned int *)realloc(A085180_s, (size_t)(nc + 1) * sizeof(unsigned int));
            A085180_cap = nc;
        }
        for (unsigned int j = 1; j <= k + 1; j++) A085180_s[++A085180_len] = j;
        A085180_next++;
    }
}

seq_t a(idx_t n) {
    unsigned long long nn = (unsigned long long)n;
    unsigned long long d = 1;
    while (d * (d + 1) / 2 < nn) d++;
    unsigned long long k = nn - d * (d - 1) / 2;
    unsigned long long x = d - k + 1, y = k;
    unsigned long long S = 0, cnt = 0;
    for (unsigned long long i = 1;; i++) {
        A085180_ensure(i);
        unsigned long long len = (unsigned long long)A085180_s[i] + 1;
        if (len >= x) {
            cnt++;
            if (cnt == y) return (seq_t)(S + x);
        }
        S += len;
    }
}
