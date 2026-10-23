/*
 * A010534  Decimal expansion of square root of 83.
 * https://oeis.org/A010534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor( sqrt(83) * 10^(n-1) ) mod 10 (n >= 1)
 * Range:     u8 n <= 9453, u16 n <= 9453, u32 n <= 9453, u64 n <= 9453,
 *            u128 n <= 9453, big any n
 * Helpers:   mul_single, sub_from
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010534 – Decimal expansion of sqrt(83) */
/* Implementation uses a digit‑by‑digit extraction algorithm that
   keeps all intermediate big integers in base‑10 arrays to avoid
   overflow in 64‑bit builds. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mul_single.h"
#include "../utils/sub_from.h"

#define BIGCAP 16384   /* capacity of the digit arrays, > 10000 */

static seq_t *digit_cache = NULL;   /* cached digits of sqrt(83) */
static size_t cache_cap = 0, cache_sz = 0;

static uint8_t P[BIGCAP];          /* current root digits (little‑endian) */
static size_t Psz;
static uint8_t R[BIGCAP];          /* current remainder digits (little‑endian) */
static size_t Rsz;

/* --- helper routines for little‑endian decimal big integers --- */
static void add_single_to(uint8_t *a, size_t *sz, uint8_t add)
{
    uint8_t carry = add;
    size_t i = 0;
    while (i < *sz && carry) {
        uint8_t sum = a[i] + carry;
        a[i] = sum % 10;
        carry = sum / 10;
        ++i;
    }
    if (carry) {
        if (*sz < BIGCAP) {
            a[(*sz)++] = carry;
        }
    }
}

static int cmp_uint8(const uint8_t *a, size_t sz1, const uint8_t *b, size_t sz2)
{
    if (sz1 > sz2) return 1;
    if (sz1 < sz2) return -1;
    for (size_t i = sz1; i > 0; --i) {
        uint8_t da = a[i - 1];
        uint8_t db = b[i - 1];
        if (da > db) return 1;
        if (da < db) return -1;
    }
    return 0;
}

static void shift_left(uint8_t *a, size_t *sz, size_t k)
{
    if (k == 0) return;
    /* make room at the beginning */
    memmove(a + k, a, (*sz) * sizeof(uint8_t));
    for (size_t i = 0; i < k; ++i) a[i] = 0;
    *sz += k;
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!digit_cache) {                    /* one‑time initialization */
        cache_cap = 16384;
        digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!digit_cache) return 0;         /* allocation failure */

        cache_sz = 1;
        digit_cache[0] = 9;                /* a(1) */

        /* initialize root (P) and remainder (R) for sqrt(83) */
        Psz = 1;
        P[0] = 9;                          /* integer part */

        Rsz = 1;
        R[0] = 2;                          /* 83 - 9*9 = 2 */
    }

    /* generate digits up to n */
    while (cache_sz < (size_t)n) {
        /* ---- compute next decimal digit ---- */

        /* R100 = R * 100 (shift left 2) */
        uint8_t R100[BIGCAP];
        size_t R100sz = Rsz;
        memcpy(R100, R, Rsz * sizeof(uint8_t));
        shift_left(R100, &R100sz, 2);

        /* m = 20 * P => (P * 2) * 10 */
        uint8_t m[BIGCAP];
        size_t msz = Psz;
        mul_single(P, Psz, 2, m, &msz);
        shift_left(m, &msz, 1);

        /* search for largest d (0..9) such that (m + d) * d <= R100 */
        int bestd = 0;
        uint8_t bestProd[BIGCAP];
        size_t bestProdsz = 0;

        for (int d = 9; d >= 0; d--) {
            uint8_t t[BIGCAP];
            size_t tsz = msz;
            memcpy(t, m, msz * sizeof(uint8_t));
            add_single_to(t, &tsz, (uint8_t)d);

            uint8_t prod[BIGCAP];
            size_t prodsz;
            mul_single(t, tsz, (uint8_t)d, prod, &prodsz);

            if (cmp_uint8(prod, prodsz, R100, R100sz) <= 0) {
                bestd = d;
                memcpy(bestProd, prod, prodsz * sizeof(uint8_t));
                bestProdsz = prodsz;
                break;
            }
        }

        /* Update remainder: R = R100 - bestProd */
        uint8_t newR[BIGCAP];
        size_t newRsiz = R100sz;
        memcpy(newR, R100, R100sz * sizeof(uint8_t));
        sub_from(newR, &newRsiz, bestProd, bestProdsz);
        memcpy(R, newR, newRsiz * sizeof(uint8_t));
        Rsz = newRsiz;

        /* Update root: P = P * 10 + bestd */
        shift_left(P, &Psz, 1);
        add_single_to(P, &Psz, (uint8_t)bestd);

        /* Cache the digit */
        if (cache_sz >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                 /* allocation failure */
            digit_cache = tmp;
            cache_cap = new_cap;
        }
        digit_cache[cache_sz++] = (seq_t)bestd;
    }

    return digit_cache[n - 1];
}
