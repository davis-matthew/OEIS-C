/*
 * A032786  Numbers k such that k(k+1)(k+2)...(k+15) /
 *          (k+(k+1)+(k+2)+...+(k+15)) is an integer.
 * https://oeis.org/A032786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 891, u64 n <= 1208,
 *            u128 n <= 1208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032786: numbers k such that k(k+1)...(k+15) / sum_{i=0..15} (k+i) is integer. 
 * Implemented by enumerating all odd divisors d ≥ 15 of N = 4108830350625 and taking
 * k = (d − 15)/2. The values are sorted in increasing order. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define N_CONST 4108830350625ULL

/* ---------- static data ---------- */
static uint64_t primes_global[32];
static unsigned  exp_global[32];
static int       prime_cnt = 0;

static seq_t *divs_global = NULL;  /* temporary array of all divisors */
static idx_t  divcnt_global = 0;

static seq_t *k_vals_global = NULL; /* final sorted array of k values */
static idx_t  kcnt_global  = 0;

/* ---------- helper functions ---------- */
static uint64_t pow_uint(uint64_t base, unsigned exp) {
    uint64_t r = 1;
    while (exp--) r *= base;
    return r;
}

static void gen_divs_rec(int idx, uint64_t cur) {
    if (idx == prime_cnt) {
        divs_global[divcnt_global++] = cur;
        return;
    }
    uint64_t mult = 1;
    for (unsigned e = 0; e <= exp_global[idx]; ++e) {
        gen_divs_rec(idx + 1, cur * mult);
        mult *= primes_global[idx];
    }
}

static void init_sequence(void) {
    if (k_vals_global != NULL) return;                     /* already initialized */

    /* factor N_CONST by trial division */
    uint64_t tmp = N_CONST;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (d == 2 || d % 2 == 1) {
            if (tmp % d == 0) {
                unsigned e = 0;
                while (tmp % d == 0) {
                    tmp /= d;
                    ++e;
                }
                primes_global[prime_cnt] = d;
                exp_global[prime_cnt]  = e;
                ++prime_cnt;
            }
        }
    }
    if (tmp > 1) {
        primes_global[prime_cnt] = tmp;
        exp_global[prime_cnt]  = 1;
        ++prime_cnt;
    }

    /* pre‑allocate array for all divisors */
    size_t max_divs = 1;
    for (int i = 0; i < prime_cnt; ++i)
        max_divs *= (size_t)(exp_global[i] + 1);
    divs_global   = (seq_t *)malloc(max_divs * sizeof(seq_t));
    divcnt_global = 0;

    /* generate all divisors */
    gen_divs_rec(0, 1u);

    /* build k list from divisors d ≥ 15 */
    k_vals_global = (seq_t *)malloc(divcnt_global * sizeof(seq_t));
    kcnt_global   = 0;
    for (idx_t i = 0; i < divcnt_global; ++i) {
        uint64_t d = (uint64_t)divs_global[i];
        if (d >= 15) {
            k_vals_global[kcnt_global++] = ((seq_t)d - 15) / 2;
        }
    }
    free(divs_global);
    divs_global = NULL;

    /* simple insertion sort (small data set) */
    for (idx_t i = 1; i < kcnt_global; ++i) {
        seq_t key = k_vals_global[i];
        idx_t j   = i;
        while (j > 0 && k_vals_global[j - 1] > key) {
            k_vals_global[j] = k_vals_global[j - 1];
            --j;
        }
        k_vals_global[j] = key;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* under‑offset, not used in tests */
    init_sequence();
    if (n - 1 >= kcnt_global) return 0;
    return k_vals_global[n - 1];
}
