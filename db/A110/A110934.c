/*
 * A110934  a(n) = A014612(n+2) - A014612(n).
 * https://oeis.org/A110934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = A014612(n+2) - A014612(n) (difference between the
 *            (n+2)-th and n-th 3‑almost primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *three_almost = NULL;
static size_t three_len = 0;
static size_t current_limit = 0;

/* Build a list of 3‑almost primes until at least need+2 terms exist. */
static void build_up_to(idx_t need) {
    if (three_len > need) return;          /* already enough data */

    size_t limit = current_limit ? current_limit * 2 : 200000;
    while (1) {
        /* smallest prime factor array */
        uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf) exit(EXIT_FAILURE);

        uint8_t *omega = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
        if (!omega) { free(spf); exit(EXIT_FAILURE); }

        for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

        /* sieve for spf */
        for (size_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if (i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }

        omega[0] = 0;  omega[1] = 0;
        for (size_t i = 2; i <= limit; ++i)
            omega[i] = 1 + omega[i / spf[i]];

        /* collect numbers with exactly 3 prime factors (with multiplicity) */
        seq_t *tmp = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
        size_t tmp_len = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (omega[i] == 3)
                tmp[tmp_len++] = (seq_t)i;

        free(spf);
        free(omega);

        if (tmp_len >= (size_t)need + 2) {
            if (three_almost) free(three_almost);
            three_almost = tmp;
            three_len = tmp_len;
            current_limit = limit;
            break;                          /* sufficient data obtained */
        } else {
            free(tmp);
            limit *= 2;                      /* increase limit and try again */
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* offset starts at 1 */
    build_up_to((idx_t)n);                  /* ensure data up to n+1 */
    size_t i = (size_t)n;                   /* 0‑based index */
    return three_almost[i + 1] - three_almost[i - 1];
}
