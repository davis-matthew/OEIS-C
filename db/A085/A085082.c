/*
 * A085082  Number of distinct prime signatures arising among the divisors of
 *          n.
 * https://oeis.org/A085082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct prime‑signature multisets of the
 *            positive divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Process: factor n, enumerate all divisor signatures, store unique
   sorted exponent lists in a dynamic table, and return its size. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* store a multiset of exponents in increasing order */
typedef struct {
    uint8_t len;
    uint8_t exps[64];
} Sig;

/* dynamic array of unique signatures used for one call of a() */
static Sig *sigs_buf = NULL;
static size_t sigs_cap = 0;
static size_t sigs_len = 0;

/* ensure capacity of the signatures buffer */
static void ensure_sigs_cap(void)
{
    if (sigs_len >= sigs_cap) {
        size_t new_cap = sigs_cap ? sigs_cap * 2 : 64;
        Sig *tmp = (Sig *)realloc(sigs_buf, new_cap * sizeof(Sig));
        if (!tmp) abort();          /* out of memory */
        sigs_buf = tmp;
        sigs_cap = new_cap;
    }
}

/* add a signature to the buffer if it is not already present */
static void add_signature(const uint8_t *cur, size_t len)
{
    for (size_t i = 0; i < sigs_len; ++i) {
        if (sigs_buf[i].len == len &&
            memcmp(sigs_buf[i].exps, cur, len) == 0)
            return;                 /* already present */
    }
    ensure_sigs_cap();
    sigs_buf[sigs_len].len = (uint8_t)len;
    memcpy(sigs_buf[sigs_len].exps, cur, len);
    ++sigs_len;
}

/* depth‑first search over the prime factors to generate all divisor
   signatures; cur holds the current signature (sorted). */
static void dfs(size_t pi, size_t pcnt, const uint8_t *exp,
                uint8_t *cur, size_t cur_len)
{
    if (pi == pcnt) {                 /* all primes processed */
        add_signature(cur, cur_len);
        return;
    }
    uint8_t e = exp[pi];
    for (uint8_t a = 0; a <= e; ++a) {
        if (a == 0) {
            dfs(pi + 1, pcnt, exp, cur, cur_len);
        } else {
            /* insert a into cur keeping it sorted */
            size_t pos = 0;
            while (pos < cur_len && cur[pos] < a) ++pos;
            for (size_t k = cur_len; k > pos; --k)
                cur[k] = cur[k - 1];
            cur[pos] = a;
            dfs(pi + 1, pcnt, exp, cur, cur_len + 1);
            /* backtrack: remove the inserted a */
            for (size_t k = pos; k < cur_len; ++k)
                cur[k] = cur[k + 1];
        }
    }
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    /* Vector of prime factors and their exponents (max 64 primes). */
    uint64_t primes[64];
    uint64_t exps64[64];
    size_t pcnt;

    factor_u64((uint64_t)n, primes, exps64, &pcnt);

    /* convert exponents to uint8_t (they are <= 255 for n up to 2^64). */
    uint8_t exp8[64];
    for (size_t i = 0; i < pcnt; ++i)
        exp8[i] = (uint8_t)exps64[i];

    /* reset signatures buffer for this call */
    sigs_len = 0;

    /* array for building current signature */
    uint8_t cur[64] = {0};

    /* enumerate all signatures */
    dfs(0, pcnt, exp8, cur, 0);

    return (seq_t)sigs_len;
}
