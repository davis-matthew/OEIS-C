/*
 * A077563  Number of partitions into two parts which have different prime
 *          signatures.
 * https://oeis.org/A077563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to split n into two positive parts, i + j
 *            = n (i <= j), such that the multiset of exponents in the prime
 *            factorizations of i and j are different
 * Range:     u8 n <= 576, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   /* provides void build_spf(idx_t limit, idx_t **spf_out); */

static idx_t *g_spf = NULL;          /* smallest prime factor table               */
static idx_t  g_spf_lim = 0;         /* maximum index the table covers           */

/* Ensure we have smallest‑prime‑factor data up to 'limit' */
static void ensure_spf(idx_t limit)
{
    if (limit < 2) return;
    if (!g_spf || g_spf_lim < limit) {
        idx_t *new_spf = NULL;
        build_spf(limit, &new_spf);
        if (g_spf) free(g_spf);
        g_spf      = new_spf;
        g_spf_lim  = limit;
    }
}

/* Factor x and fill buf[] with its prime‑factor exponents
   (sorted ascending).  Return the number of exponents. */
static int factor_exponents(idx_t x, int *buf)
{
    int cnt = 0;
    while (x > 1) {
        idx_t p = g_spf[x];
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        buf[cnt++] = e;
    }
    /* Insertion sort for very small arrays */
    for (int i = 1; i < cnt; ++i) {
        int key = buf[i];
        int j = i - 1;
        while (j >= 0 && buf[j] > key) {
            buf[j + 1] = buf[j];
            --j;
        }
        buf[j + 1] = key;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* no valid partitions for n=0 or 1 */

    ensure_spf(n);

    seq_t result = 0;
    int sig_i[32], sig_j[32];
    int len_i, len_j;

    for (idx_t i = 1; i <= n / 2; ++i) {
        idx_t j = n - i;
        len_i = factor_exponents(i, sig_i);
        len_j = factor_exponents(j, sig_j);

        bool sig_equal = (len_i == len_j);
        if (sig_equal) {
            for (int k = 0; k < len_i; ++k) {
                if (sig_i[k] != sig_j[k]) {
                    sig_equal = false;
                    break;
                }
            }
        }
        if (!sig_equal) {
            ++result;
        }
    }
    return result;
}
