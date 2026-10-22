/*
 * A006308  Coefficients of period polynomials.
 * https://oeis.org/A006308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p-1)/2 where p = prime(n) = A000040(n) (A008837)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *a6308_comp = NULL;
static uint64_t a6308_lim = 0;
static uint64_t *a6308_primes = NULL;
static uint64_t a6308_np = 0;

static void a6308_sieve(uint64_t lim) {
    uint64_t i, j;
    free(a6308_comp);
    free(a6308_primes);
    a6308_comp = (unsigned char *)malloc((size_t)(lim + 1));
    memset(a6308_comp, 0, (size_t)(lim + 1));
    a6308_comp[0] = 1;
    if (lim >= 1) a6308_comp[1] = 1;
    for (i = 2; i * i <= lim; i++)
        if (!a6308_comp[i])
            for (j = i * i; j <= lim; j += i)
                a6308_comp[j] = 1;
    a6308_np = 0;
    for (i = 2; i <= lim; i++)
        if (!a6308_comp[i]) a6308_np++;
    a6308_primes = (uint64_t *)malloc((size_t)(a6308_np + 1) * sizeof(uint64_t));
    a6308_np = 0;
    for (i = 2; i <= lim; i++)
        if (!a6308_comp[i]) a6308_primes[a6308_np++] = i;
    a6308_lim = lim;
}

static uint64_t a6308_nth_prime(uint64_t n) {
    uint64_t lim = a6308_lim ? a6308_lim : 1024;
    while (a6308_np < n) {
        if (a6308_lim) lim *= 2;
        a6308_sieve(lim);
    }
    return a6308_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t pr = a6308_nth_prime((uint64_t)n);
    seq_t p = (seq_t)pr;
    seq_t q = (seq_t)(pr - 1);
    if (pr % 2 == 0)
        return (p / 2) * q;
    return p * (q / 2);
}
