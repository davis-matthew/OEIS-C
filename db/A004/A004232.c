/*
 * A004232  a(n) = n^2 + prime(n).
 * https://oeis.org/A004232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + prime(n)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *pr_list = NULL;
static idx_t pr_count = 0;
static uint64_t pr_limit = 0;

static void sieve_to(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i)
                comp[j] = 1;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) cnt++;
    free(pr_list);
    pr_list = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    pr_count = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) pr_list[pr_count++] = i;
    free(comp);
    pr_limit = limit;
}

static uint64_t nth_prime(idx_t n) {
    uint64_t limit = pr_limit ? pr_limit : 1024;
    while (pr_count < n) {
        if (pr_limit) limit *= 2;
        sieve_to(limit);
    }
    return pr_list[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = nth_prime(n);
    return (seq_t)n * (seq_t)n + (seq_t)p;
}
