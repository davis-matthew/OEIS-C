/*
 * A004789  Least k such that number of distinct prime divisors of the numbers
 *          in row k of Pascal's triangle is n.
 * https://oeis.org/A004789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. A004788(k) = pi(k) - e(k), where e(k) = 1 if k+1 =
 *            m*P^a with P the largest prime factor of k+1, P^a exactly
 *            dividing k+1, 1 <= m < P and P <= k (Lucas: P then divides no
 *            C(k,j)); otherwise e(k) = 0. a(n) = least k with A004788(k) = n
 * Range:     u8 n <= 54, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf_tab = 0;
static long *first_tab = 0;
static long first_len = 0;
static long cur_lim = 0;

static int excl(uint64_t x) {
    uint64_t y = x, P = 0, pa = 1;
    if (x < 2) return 0;
    while (y > 1) {
        uint64_t p = spf_tab[y], q = 1;
        while (y % p == 0) { y /= p; q *= p; }
        if (p > P) { P = p; pa = q; }
    }
    if (P <= x - 1 && x / pa < P) return 1;
    return 0;
}

static void build(long L) {
    long i, j, k, pi = 0;
    free(spf_tab);
    free(first_tab);
    spf_tab = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(L + 2));
    for (i = 0; i <= L + 1; i++) spf_tab[i] = 0;
    for (i = 2; i <= L + 1; i++) {
        if (spf_tab[i] == 0) {
            for (j = i; j <= L + 1; j += i)
                if (spf_tab[j] == 0) spf_tab[j] = (uint32_t)i;
        }
    }
    first_len = L + 2;
    first_tab = (long *)malloc(sizeof(long) * (size_t)first_len);
    for (i = 0; i < first_len; i++) first_tab[i] = -1;
    for (k = 0; k <= L; k++) {
        long val;
        if (k >= 2 && spf_tab[k] == (uint32_t)k) pi++;
        val = pi - excl((uint64_t)k + 1);
        if (val >= 0 && val < first_len && first_tab[val] < 0) first_tab[val] = k;
    }
    cur_lim = L;
}

seq_t a(idx_t n) {
    if (cur_lim == 0) build(1024);
    while ((long long)n >= (long long)first_len || first_tab[(long)n] < 0)
        build(cur_lim * 2);
    return (seq_t)first_tab[(long)n];
}
