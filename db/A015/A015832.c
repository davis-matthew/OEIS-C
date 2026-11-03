/*
 * A015832  Numbers k such that phi(k + 12) | sigma(k).
 * https://oeis.org/A015832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive k such that φ(k+12) divides σ(k)
 * Range:     u8 n <= 17, u16 n <= 676, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 71 terms through n = 1681 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* compute σ(n) for 64‑bit n */
static seq_t sigma_u64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t m  = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t pk   = 1;
            uint64_t sum  = 1;
            while (m % p == 0) {
                m   /= p;
                pk  *= p;
                sum += pk;
            }
            res *= sum;
        }
    }
    if (m > 1) res *= (1 + m);
    return (seq_t)res;
}

/* compute φ(n) for 64‑bit n */
static seq_t phi_u64(uint64_t n)
{
    uint64_t result = n;
    uint64_t m      = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t processed_k = 0;

    if (n == 0) return 0;                    /* offset 1, guard just in case */

    while (terms_len < (size_t)n) {
        ++processed_k;
        seq_t sigma_k = sigma_u64((uint64_t)processed_k);
        seq_t phi_k12 = phi_u64((uint64_t)(processed_k + 12));
        if (phi_k12 != 0 && sigma_k % phi_k12 == 0) {
            append_candidate(&terms, &terms_len, &terms_cap, (seq_t)processed_k);
        }
    }
    return terms[(size_t)n - 1];
}
