/*
 * A091009  Number of triples (u,v,w) of divisors of n with v-u = w-v, and u <
 *          v < w.
 * https://oeis.org/A091009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (u,v,w) of divisors of n with u < v < w
 *            and v - u = w - v
 * Range:     u8 n <= 901, u16 n <= 9668, u32 n <= 9668, u64 n <= 9668,
 *            u128 n <= 9668, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compare function for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t u = *(const uint64_t *)a;
    uint64_t v = *(const uint64_t *)b;
    return (u < v) ? -1 : (u > v) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N = (uint64_t)n;

    /* collect divisors */
    size_t cap = 32;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) return 0;          /* allocation failure – return 0 */
    size_t dcnt = 0;

    /* naive divisor enumeration up to sqrt(N) */
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            if (dcnt >= cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                if (!tmp) { free(divs); return 0; } /* allocation failure */
                divs = tmp;
            }
            divs[dcnt++] = i;
            uint64_t q = N / i;
            if (q != i) {
                if (dcnt >= cap) {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[dcnt++] = q;
            }
        }
    }

    /* sort divisors */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* count 3‑term arithmetic progressions */
    seq_t result = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        for (size_t j = i + 1; j < dcnt; ++j) {
            uint64_t u = divs[i];
            uint64_t v = divs[j];
            seq_t w = ((seq_t)2 * (seq_t)v) - (seq_t)u;
            if (w <= (seq_t)N && ((seq_t)N % w == 0)) {
                /* w > v automatically since u < v */
                result++;
            }
        }
    }

    free(divs);
    return result;
}
