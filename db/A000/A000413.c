/*
 * A000413  Let A(n) = #{(i,j,k): i^2 + j^2 + k^2 <= n}, V(n) =
 *          (4/3)*Pi*n^(3/2), P(n) = A(n) - V(n); A000092 gives values of n
 *          where |P(n)| sets a new record; sequence gives A(A000092(n)).
 * https://oeis.org/A000413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search over m = 0,1,2,...: A(m) = A(m-1) + r3(m), with r3(m) =
 *            sum_k r2(m-k^2) and r2 sieved; record whenever |A(m) -
 *            (4/3)*Pi*m^(3/2)| beats all earlier values; a(n) = A(m) at the
 *            n-th record, a(0) = A(0) = 1
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t *rec_vals = 0;
static size_t nrec = 0, caprec = 0;
static uint64_t cur_m = 0, curA = 1;
static long double maxP = 0.0L;

static void push_rec(uint64_t v) {
    if (nrec == caprec) {
        caprec = caprec ? caprec * 2 : 64;
        rec_vals = (uint64_t *)realloc(rec_vals, caprec * sizeof(uint64_t));
    }
    rec_vals[nrec++] = v;
}

static void extend(void) {
    const long double PI = 3.14159265358979323846264338327950288L;
    uint64_t N = cur_m < 2048 ? 4096 : cur_m * 2;
    uint32_t *r2 = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint64_t i, j, m, k;
    for (i = 0; i * i <= N; i++) {
        uint32_t wi = i ? 2 : 1;
        for (j = 0; i * i + j * j <= N; j++)
            r2[i * i + j * j] += wi * (j ? 2 : 1);
    }
    for (m = cur_m + 1; m <= N; m++) {
        uint64_t s = r2[m];
        for (k = 1; k * k <= m; k++) s += 2 * (uint64_t)r2[m - k * k];
        curA += s;
        long double V = (4.0L / 3.0L) * PI * (long double)m * sqrtl((long double)m);
        long double P = fabsl((long double)curA - V);
        if (P > maxP) {
            maxP = P;
            push_rec(curA);
        }
    }
    cur_m = N;
    free(r2);
}

seq_t a(idx_t n) {
    if (nrec == 0) push_rec(1);
    while (nrec <= n) extend();
    return (seq_t)rec_vals[n];
}
