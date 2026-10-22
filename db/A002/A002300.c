/*
 * A002300  Coefficients in the expansion of B^2*C^3 in Watson's notation of
 *          page 118.
 * https://oeis.org/A002300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f. = Product_{k>0} (1-x^k)^2 * (1-x^(7k))^3, computed as a(n)
 *            = Sum_{m>=0} (-1)^m*(2m+1) * e2(n - 7*m*(m+1)/2), where e2 =
 *            coefficients of eta(q)^2 = (Sum_k (-1)^k x^(k(3k-1)/2))^2 (Euler
 *            pentagonal + Jacobi triple product)
 * Range:     i8 n <= 199, i16 n <= 199, i32 n <= 199, i64 n <= 199,
 *            i128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long *e2_tab = NULL;
static idx_t e2_cap = 0;

static void ensure_e2(idx_t n) {
    if (n + 1 <= e2_cap) return;
    idx_t newcap = n + 1;
    if (newcap < 2 * e2_cap) newcap = 2 * e2_cap;
    if (newcap < 64) newcap = 64;

    int *P = (int *)calloc((size_t)newcap, sizeof(int));
    idx_t *pg = (idx_t *)malloc(sizeof(idx_t) * 2 * 2048);
    int *ps = (int *)malloc(sizeof(int) * 2 * 2048);
    size_t np = 0, pcap = 2 * 2048;
    for (idx_t k = 0;; k++) {
        idx_t g1 = k * (3 * k - 1) / 2;
        if (k == 0) g1 = 0;
        if (g1 >= newcap) break;
        int s = (k & 1) ? -1 : 1;
        if (np + 2 > pcap) {
            pcap *= 2;
            pg = (idx_t *)realloc(pg, sizeof(idx_t) * pcap);
            ps = (int *)realloc(ps, sizeof(int) * pcap);
        }
        P[g1] = s;
        pg[np] = g1; ps[np] = s; np++;
        if (k > 0) {
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 < newcap) {
                P[g2] = s;
                pg[np] = g2; ps[np] = s; np++;
            }
        }
    }

    long long *E = (long long *)malloc(sizeof(long long) * (size_t)newcap);
    for (idx_t j = 0; j < newcap; j++) {
        long long sum = 0;
        for (size_t i = 0; i < np; i++) {
            if (pg[i] <= j) sum += (long long)ps[i] * P[j - pg[i]];
        }
        E[j] = sum;
    }
    free(P); free(pg); free(ps);
    free(e2_tab);
    e2_tab = E;
    e2_cap = newcap;
}

seq_t a(idx_t n) {
    ensure_e2(n);
    seq_t r = 0;
    for (idx_t m = 0;; m++) {
        idx_t t = 7 * m * (m + 1) / 2;
        if (t > n) break;
        long long c = (long long)(2 * m + 1);
        if (m & 1) c = -c;
        long long term = e2_tab[n - t] * c;
        if (term != 0) r += (seq_t)term;
    }
    return r;
}
