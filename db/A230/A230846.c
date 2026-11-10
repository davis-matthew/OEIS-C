/*
 * A230846  1 + A075526(n).
 * https://oeis.org/A230846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A075526(n-1), where A075526(0) = 1 and A075526(m) =
 *            prime(m+1) - prime(m) for m >= 1; so a(1) = 2 and a(n) = 1 +
 *            prime(n) - prime(n-1) for n >= 2. Computed with a sieve of
 *            Eratosthenes
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 97 terms through n = 97 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a230846_pr = NULL;
static uint64_t a230846_np = 0;
static uint64_t a230846_lim = 0;

static void a230846_build(uint64_t lim)
{
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)(lim + 1));
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i)
                comp[j] = 1;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            cnt++;
    free(a230846_pr);
    a230846_pr = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            a230846_pr[cnt++] = i;
    free(comp);
    a230846_np = cnt;
    a230846_lim = lim;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;
    uint64_t lim = a230846_lim ? a230846_lim : 128;
    while (a230846_np < (uint64_t)n) {
        lim *= 2;
        a230846_build(lim);
    }
    return (seq_t)(a230846_pr[n - 1] - a230846_pr[n - 2] + 1);
}
