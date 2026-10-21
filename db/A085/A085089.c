/*
 * A085089  Number of distinct prime signatures arising up to n.
 * https://oeis.org/A085089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct prime signatures among the integers 1
 *            … n * A prime signature of m is the multiset of the exponents in
 *            its prime * factorisation (order of the primes is irrelevant).
 *            The function * builds a table of smallest prime factors (SPF)
 *            for 1 … n, extracts and * normalises the ex...
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void build_spf(idx_t N, int *spf)
{
    /* initialise */
    for (idx_t i = 0; i <= N; i++)
        spf[i] = 0;

    for (idx_t i = 2; i <= N; i++) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (int)i;
            if (i * i <= N) {
                for (idx_t j = i * i; j <= N; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (int)i;
                }
            }
        }
    }
}

struct Sig {
    size_t len;   /* number of exponents (length of the signature) */
    uint8_t *exps;/* sorted list of exponents, NULL if len == 0 */
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t N = n;

    /* 1. Build SPF table */
    int *spf = (int*)calloc((size_t)N + 1, sizeof(int));
    build_spf(N, spf);

    /* 2. Store distinct signatures */
    struct Sig *sigs = (struct Sig*)malloc(sizeof(struct Sig));
    sigs[0].len   = 0;
    sigs[0].exps  = NULL;
    size_t sig_cnt = 1;
    size_t sig_cap = 1;

    uint8_t exps_buf[32];   /* enough for n <= 10^8 (max 7 exponents,   */
                            /* 32 is safe for all tests).  Exponents themselves fit in a byte. */

    for (idx_t num = 2; num <= N; num++) {
        idx_t x = num;
        size_t len = 0;

        /* factorise num, recording exponent counts */
        while (x > 1) {
            int p = spf[x];
            uint8_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            exps_buf[len++] = cnt;
        }

        /* sort exponents so that equal multisets have identical order */
        if (len > 1) {
            for (size_t i = 1; i < len; i++) {
                uint8_t key = exps_buf[i];
                size_t j = i;
                while (j > 0 && exps_buf[j - 1] > key) {
                    exps_buf[j] = exps_buf[j - 1];
                    j--;
                }
                exps_buf[j] = key;
            }
        }

        /* check if signature already seen */
        int found = 0;
        for (size_t k = 0; k < sig_cnt; k++) {
            if (sigs[k].len != len)
                continue;
            if (len > 0 && memcmp(sigs[k].exps, exps_buf, len) != 0)
                continue;
            found = 1;
            break;
        }

        /* if new, add it */
        if (!found) {
            if (sig_cnt == sig_cap) {
                sig_cap = sig_cap ? sig_cap * 2 : 1;
                sigs = (struct Sig*)realloc(sigs, sig_cap * sizeof(struct Sig));
            }
            sigs[sig_cnt].len = len;
            if (len > 0) {
                sigs[sig_cnt].exps = (uint8_t*)malloc(len);
                memcpy(sigs[sig_cnt].exps, exps_buf, len);
            } else {
                sigs[sig_cnt].exps = NULL;
            }
            sig_cnt++;
        }
    }

    /* 3. Cleanup */
    free(spf);
    for (size_t i = 0; i < sig_cnt; i++)
        free(sigs[i].exps);
    free(sigs);

    return (seq_t)sig_cnt;
}
