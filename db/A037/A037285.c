/*
 * A037285  Replace n with concatenation of its nontrivial odd divisors.
 * https://oeis.org/A037285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the concatenation of the non‑trivial odd divisors of n
 *            (i.e., the odd divisors d with 1 < d < n), sorted in ascending
 *            order. If n has no such divisors, a(n) = 0
 * Range:     u8 n <= 29, u16 n <= 74, u32 n <= 209, u64 n <= 629,
 *            u128 n <= 2834, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow10.h"     /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

static int cmp_uint64_asc(const void *p1, const void *p2)
{
    uint64_t v1 = *(const uint64_t *)p1;
    uint64_t v2 = *(const uint64_t *)p2;
    /* ascending order: negative if v1 < v2 */
    return (v1 > v2) - (v1 < v2);
}

seq_t a(idx_t n)
{
    /* Extract the odd part of n. */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL)
        m >>= 1;
    if (m <= 1ULL)          /* no non‑trivial odd divisors */
        return (seq_t)0;

    /* Factorise the odd part. */
    uint64_t primes[32];
    int exps[32];
    int pcnt = 0;
    uint64_t tmp = m;
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Generate all divisors of the odd part. */
    size_t divcap = 16;
    size_t divlen = 1;
    uint64_t *divs = (uint64_t *)malloc(divcap * sizeof(uint64_t));
    if (!divs)
        return (seq_t)0;      /* OOM safety */
    divs[0] = 1;

    for (int i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int e  = exps[i];
        size_t oldlen = divlen;
        for (size_t j = 0; j < oldlen; ++j) {
            uint64_t base = divs[j];
            for (int k = 1; k <= e; ++k) {
                base *= p;
                if (divlen == divcap) {
                    divcap <<= 1;
                    uint64_t *tmpPtr = (uint64_t *)realloc(divs, divcap * sizeof(uint64_t));
                    if (!tmpPtr) {
                        free(divs);
                        return (seq_t)0;
                    }
                    divs = tmpPtr;
                }
                divs[divlen++] = base;
            }
        }
    }

    /* Collect proper odd divisors (exclude 1 and n). */
    size_t propcap = 16;
    size_t proclen = 0;
    uint64_t *proper = (uint64_t *)malloc(propcap * sizeof(uint64_t));
    if (!proper) {
        free(divs);
        return (seq_t)0;
    }
    for (size_t i = 0; i < divlen; ++i) {
        uint64_t d = divs[i];
        if (d > 1 && d < (uint64_t)n) {
            if (proclen == propcap) {
                propcap <<= 1;
                uint64_t *tmpPtr = (uint64_t *)realloc(proper, propcap * sizeof(uint64_t));
                if (!tmpPtr) {
                    free(divs);
                    free(proper);
                    return (seq_t)0;
                }
                proper = tmpPtr;
            }
            proper[proclen++] = d;
        }
    }
    free(divs);

    if (proclen == 0) {
        free(proper);
        return (seq_t)0;
    }

    /* Sort the proper divisors ascending. */
    qsort(proper, proclen, sizeof(uint64_t), cmp_uint64_asc);

    /* Concatenate the sorted divisors. */
    seq_t res = (seq_t)0;
    for (size_t i = 0; i < proclen; ++i) {
        uint64_t d = proper[i];
        int digits = digit_len((seq_t)d);
        seq_t mult = pow10((idx_t)digits);
        res = res * mult + (seq_t)d;
    }
    free(proper);
    return res;
}
