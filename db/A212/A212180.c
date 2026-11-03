/*
 * A212180  Number of distinct second signatures (cf. A212172) represented
 *          among divisors of n.
 * https://oeis.org/A212180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct second signatures among divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 8044 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* recursive enumeration of all exponent combinations for the given prime factors */
static void dfs_recurse(size_t depth, size_t num_factors,
                        const uint64_t *max_exp, uint64_t *cur_exp,
                        char ***signatures, size_t *cnt, size_t *cap)
{
    if (depth == num_factors) {
        /* collect exponents >= 2 for this divisor */
        uint8_t sig[16];
        size_t sig_len = 0;
        for (size_t i = 0; i < num_factors; ++i) {
            if (cur_exp[i] >= 2) {
                sig[sig_len++] = (uint8_t)cur_exp[i];
            }
        }

        /* sort the exponents to obtain a canonical string representation */
        for (size_t i = 0; i < sig_len; ++i) {
            for (size_t j = i + 1; j < sig_len; ++j) {
                if (sig[j] < sig[i]) {
                    uint8_t tmp = sig[i];
                    sig[i] = sig[j];
                    sig[j] = tmp;
                }
            }
        }

        /* encode into a null‑terminated string */
        char buf[64];
        size_t pos = 0;
        for (size_t i = 0; i < sig_len; ++i) {
            if (i) buf[pos++] = ',';
            int written = sprintf(buf + pos, "%u", sig[i]);
            pos += written;
        }
        buf[pos] = '\0';

        /* check for duplicate signatures */
        for (size_t j = 0; j < *cnt; ++j) {
            if (strcmp(buf, (*signatures)[j]) == 0) return; /* duplicate */
        }

        /* store new signature */
        if (*cnt == *cap) {
            size_t new_cap = (*cap * 2) ? (*cap * 2) : 4;
            char **new_arr = (char **)realloc(*signatures,
                                              new_cap * sizeof(char *));
            if (!new_arr) return; /* allocation failed, abort this branch */
            *signatures = new_arr;
            *cap = new_cap;
        }
        (*signatures)[*cnt] = (char *)malloc(pos + 1);
        if (!(*signatures)[*cnt]) return;
        memcpy((*signatures)[*cnt], buf, pos + 1);
        (*cnt)++;
        return;
    }

    for (uint64_t e = 0; e <= max_exp[depth]; ++e) {
        cur_exp[depth] = e;
        dfs_recurse(depth + 1, num_factors, max_exp, cur_exp,
                    signatures, cnt, cap);
    }
}

/* factorise a uint64_t n into prime bases and exponents (max 16 factors) */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    if (n == 0) { *cnt = 0; return; }

    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) { e++; n /= 2; }
        primes[c] = 2;
        exps[c]  = e;
        ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { e++; n /= p; }
            primes[c] = p;
            exps[c]  = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* public function as required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behaviour for indices < 1 is irrelevant */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[16], exps[16];
    size_t num_factors = 0;
    factor_uint64(nn, primes, exps, &num_factors);

    uint64_t cur_exp[16] = {0};

    size_t cap = 4;
    size_t cnt = 0;
    char **signatures = (char **)malloc(cap * sizeof(char *));
    if (!signatures) return 0;

    dfs_recurse(0, num_factors, exps, cur_exp,
                &signatures, &cnt, &cap);

    for (size_t i = 0; i < cnt; ++i) free(signatures[i]);
    free(signatures);

    return (seq_t)cnt;
}
