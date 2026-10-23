/*
 * A115931  Numbers k such that k and 7*k, taken together, are zeroless
 *          pandigital.
 * https://oeis.org/A115931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   brute‑force search of all 4‑digit numbers k such that 7*k is a
 *            5‑digit number and the 9 digits of k and 7*k together form a
 *            zero‑less pandigital set {1,…,9}. The function returns the n‑th
 *            such number with offset 1
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* comparison function for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* static cache of solution set */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* allocate buffer for candidates */
        size_t cap = 64;
        seq_t *buf = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t len = 0;

        /* generate all 3024 permutations of 4 distinct digits 1–9 */
        for (int d1 = 1; d1 <= 9; ++d1) {
            for (int d2 = 1; d2 <= 9; ++d2) {
                if (d2 == d1) continue;
                for (int d3 = 1; d3 <= 9; ++d3) {
                    if (d3 == d1 || d3 == d2) continue;
                    for (int d4 = 1; d4 <= 9; ++d4) {
                        if (d4 == d1 || d4 == d2 || d4 == d3) continue;

                        uint64_t k = (uint64_t)d1 * 1000ULL
                                   + (uint64_t)d2 * 100ULL
                                   + (uint64_t)d3 * 10ULL
                                   + (uint64_t)d4;
                        uint64_t prod = 7ULL * k;

                        /* 7*k must be 5 digits */
                        if (prod < 10000ULL || prod > 99999ULL)
                            continue;

                        /* compute mask of digits of k */
                        unsigned maskk = (1U << d1) | (1U << d2) |
                                         (1U << d3) | (1U << d4);

                        /* compute mask of digits of 7*k */
                        unsigned maskp = 0;
                        uint64_t tmp = prod;
                        bool ok = true;
                        for (int i = 0; i < 5; ++i) {
                            unsigned dig = (unsigned)(tmp % 10ULL);
                            tmp /= 10ULL;
                            if (dig == 0U || (maskp & (1U << dig))) {
                                ok = false;
                                break;
                            }
                            maskp |= 1U << dig;
                        }
                        if (!ok) continue;

                        /* digits must be disjoint and cover all 1–9 */
                        if ((maskk & maskp) || ((maskk | maskp) != 0x3FEU))
                            continue;

                        /* store k */
                        if (len == cap) {
                            cap <<= 1;
                            buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
                        }
                        buf[len++] = (seq_t)k;
                    }
                }
            }
        }

        /* sort the candidates */
        qsort(buf, len, sizeof(seq_t), cmp_seq_t);

        /* keep cache */
        cache = buf;
        cache_len = len;
        initialized = true;
    }

    if (n == 0)
        return 0; /* undefined for n<1, but return 0 */

    if ((idx_t)(n - 1) < cache_len)
        return cache[n - 1];
    return 0; /* out of range */
}
