/*
 * A005867  a(0) = 1; for n > 0, a(n) = (prime(n)-1)*a(n-1).
 * https://oeis.org/A005867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product_{k=1..n} (prime(k)-1), i.e. a(0) = 1, a(n) =
 *            (prime(n)-1)*a(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *primes_tab = 0;
static idx_t primes_cnt = 0;

static void ensure_primes(idx_t n) {
    uint64_t limit;
    unsigned char *comp;
    uint64_t i, j;
    idx_t cnt;

    if (primes_cnt >= n) return;

    limit = 64;
    for (;;) {
        /* pi(x) >= x / (ln x + 2) roughly; just sieve and double until enough */
        comp = (unsigned char *)malloc((size_t)limit + 1);
        memset(comp, 0, (size_t)limit + 1);
        cnt = 0;
        for (i = 2; i <= limit; i++) {
            if (!comp[i]) {
                cnt++;
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        if (cnt >= n) break;
        free(comp);
        limit *= 2;
    }

    free(primes_tab);
    primes_tab = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
    primes_cnt = 0;
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) primes_tab[primes_cnt++] = i;
    }
    free(comp);
}

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t k;

    if (n == 0) return result;

    ensure_primes(n);

    for (k = 0; k < n; k++) {
        result *= (seq_t)(primes_tab[k] - 1);
    }
    return result;
}
