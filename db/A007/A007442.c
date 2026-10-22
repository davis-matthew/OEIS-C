/*
 * A007442  Inverse binomial transform of primes.
 * https://oeis.org/A007442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^(n-k-1) binomial(n-1, k) prime(k+1),
 *            i.e. the (n-1)-st difference of the first n primes, computed via
 *            an incrementally updated difference-table diagonal
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 34, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   grow_zero, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/grow_zero.h"

static seq_t *diag = NULL;   /* diag[j] = j-th difference ending at the latest prime */
static seq_t *res = NULL;    /* res[m] = a(m+1) */
static idx_t cnt = 0;        /* number of primes processed */
static idx_t cap = 0;
static uint64_t lastp = 1;

static uint64_t next_prime(uint64_t p) {
    uint64_t q = p + 1;
    while (!is_prime(q)) q++;
    return q;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (cnt < n) {
        if (cnt >= cap) {
            idx_t newcap = cap ? 2 * cap : 64;
            diag = grow_zero(diag, cap, newcap);
            res = grow_zero(res, cap, newcap);
            cap = newcap;
        }
        lastp = next_prime(lastp);
        seq_t prev = diag[0];
        diag[0] = (seq_t)lastp;
        for (idx_t j = 1; j <= cnt; j++) {
            seq_t tmp = diag[j];
            diag[j] = diag[j - 1] - prev;
            prev = tmp;
        }
        res[cnt] = diag[cnt];
        cnt++;
    }
    return res[n - 1];
}
