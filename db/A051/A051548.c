/*
 * A051548  a(n) = LCM { tau(1), ..., tau(n) }.
 * https://oeis.org/A051548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM { tau(1), …, tau(n) }
 * Range:     u8 n <= 35, u16 n <= 1023, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   gcd, tau_from_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/tau_from_spf.h"

static idx_t     max_n = 0;                /* largest n already evaluated   */
static seq_t    *a_arr = NULL;            /* a(n) values cache             */
static seq_t      lcm_val = 1;            /* current LCM of tau(1)..tau(k) */
static uint32_t *spf   = NULL;            /* smallest prime factor array   */
static idx_t      spf_len = 0;            /* size of spf array             */

/* Build smallest prime factor table up to N (inclusive) */
static void build_spf(idx_t N, uint32_t *table) {
    for (idx_t i = 0; i <= N; ++i) table[i] = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (table[i] == 0) {
            table[i] = (uint32_t)i;
            if (i * i <= N) {
                for (idx_t j = i * i; j <= N; j += i)
                    if (table[j] == 0) table[j] = (uint32_t)i;
            }
        }
    }
}

seq_t a(idx_t n) {
    /* initial allocation on first call */
    if (a_arr == NULL) {
        a_arr = (seq_t *)malloc(sizeof(seq_t));
        a_arr[0] = (seq_t)1;
    }

    /* Return cached value if already computed */
    if (n <= max_n) return a_arr[n];

    /* Ensure spf table is large enough */
    if (n + 1 > spf_len) {
        idx_t new_size = n + 1;
        uint32_t *new_spf = (uint32_t *)realloc(spf, new_size * sizeof(uint32_t));
        spf = new_spf;
        build_spf(new_size - 1, spf);
        spf_len = new_size;
    }

    /* Expand a_arr to hold up to a[n] */
    if (n + 1 > max_n + 1) {
        seq_t *new_a = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
        a_arr = new_a;
    }

    /* Compute a(k) for k = max_n+1 .. n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        uint32_t tau_k = tau_from_spf(k, spf);
        seq_t   tau   = (seq_t)tau_k;

        /* lcm_val = lcm(lcm_val, tau_k) = lcm_val / gcd(lcm_val, tau) * tau */
        seq_t g   = gcd(lcm_val, tau);
        seq_t tmp = lcm_val / g;
        lcm_val   = tmp * tau;

        a_arr[k] = lcm_val;
    }

    max_n = n;
    return a_arr[n];
}
