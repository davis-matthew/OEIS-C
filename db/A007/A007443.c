/*
 * A007443  Binomial transform of primes.
 * https://oeis.org/A007443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1)*prime(k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *pr_list = NULL;
static idx_t pr_count = 0;

static void ensure_primes(idx_t k) {
    uint64_t limit;
    if (pr_count >= k) return;
    limit = 1024;
    for (;;) {
        unsigned char *comp = (unsigned char *)malloc((size_t)limit + 1);
        idx_t cnt = 0;
        uint64_t i, j;
        memset(comp, 0, (size_t)limit + 1);
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
        for (i = 2; i <= limit; i++)
            if (!comp[i]) cnt++;
        if (cnt >= k) {
            idx_t t = 0;
            free(pr_list);
            pr_list = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)cnt);
            for (i = 2; i <= limit; i++)
                if (!comp[i]) pr_list[t++] = i;
            pr_count = cnt;
            free(comp);
            return;
        }
        free(comp);
        limit *= 2;
    }
}

static uint64_t small_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t m, j;
    seq_t c, result;
    ensure_primes(n);
    m = n - 1;
    c = 1;              /* binomial(m, j) */
    result = 0;
    for (j = 0; j <= m; j++) {
        result += c * (seq_t)pr_list[j];
        if (j < m) {
            uint64_t num = (uint64_t)(m - j);
            uint64_t den = (uint64_t)(j + 1);
            uint64_t g = small_gcd(num, den);
            c = c / (seq_t)(den / g);
            c = c * (seq_t)(num / g);
        }
    }
    return result;
}
