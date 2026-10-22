/*
 * A007797  List of pairs of primes in reverse order.
 * https://oeis.org/A007797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(prime(2n), prime(2n-1)), i.e. a(n) =
 *            prime(2n)*10^len(prime(2n-1)) + prime(2n-1); primes found by a
 *            sieve
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *a7797_pr = NULL;
static idx_t a7797_npr = 0;
static uint64_t a7797_lim = 0;

static void a7797_ensure(idx_t k) {
    while (a7797_npr < k) {
        uint64_t lim = a7797_lim ? a7797_lim * 2 : 4096;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j;
        idx_t cnt = 0;
        uint64_t *pr;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 2; i <= lim; i++) if (!comp[i]) cnt++;
        pr = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
        cnt = 0;
        for (i = 2; i <= lim; i++) if (!comp[i]) pr[cnt++] = i;
        free(comp);
        free(a7797_pr);
        a7797_pr = pr;
        a7797_npr = cnt;
        a7797_lim = lim;
    }
}

seq_t a(idx_t n) {
    uint64_t p, q, t;
    seq_t mult = 1;
    a7797_ensure(2 * n);
    p = a7797_pr[2 * n - 2];
    q = a7797_pr[2 * n - 1];
    for (t = p; t > 0; t /= 10) mult *= 10;
    return (seq_t)q * mult + (seq_t)p;
}
