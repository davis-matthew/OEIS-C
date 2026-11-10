/*
 * A064359  Inverse of sequence A052331 considered as a permutation of the
 *          natural numbers.
 * https://oeis.org/A064359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A052330(n) - 1, where A052330(n) is the product of the
 *            Fermi-Dirac primes (the numbers p^(2^k), in increasing order:
 *            2,3,4,5,7,9,...) b_i over each bit i set in n; computed by a
 *            small sieve for the Fermi-Dirac primes
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t fd_b[64];
static int fd_ready = 0;

static int fd_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static void fd_init(void)
{
    if (fd_ready) return;
    const int LIM = 1000;
    static uint64_t vals[2000];
    int cnt = 0;
    for (int p = 2; p <= LIM; ++p) {
        int prime = 1;
        for (int d = 2; d * d <= p; ++d)
            if (p % d == 0) { prime = 0; break; }
        if (!prime) continue;
        uint64_t v = (uint64_t)p;
        while (v <= (uint64_t)LIM) {
            vals[cnt++] = v;
            v = v * v;
        }
    }
    qsort(vals, (size_t)cnt, sizeof(uint64_t), fd_cmp);
    for (int i = 0; i < 64; ++i) fd_b[i] = vals[i];
    fd_ready = 1;
}

seq_t a(idx_t n)
{
    fd_init();
    seq_t res = 1;
    for (int i = 0; i < 64; ++i) {
        if ((n >> i) & 1U)
            res *= (seq_t)fd_b[i];
    }
    return res - 1;
}
