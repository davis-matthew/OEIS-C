/*
 * A078094  Numbers k such that Omega(k) = Omega(k+1) + Omega(k+2) +
 *          Omega(k+3) + Omega(k+4) where Omega(k) denotes the number of prime
 *          factors of k, counting multiplicity.
 * https://oeis.org/A078094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k with Ω(k)=Ω(k+1)+Ω(k+2)+Ω(k+3)+Ω(k+4)
 * Range:     u8 none, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000000ULL        /* upper bound for sieve (≈20 000 000) */

static uint32_t *spf = NULL;      /* smallest prime factor for each i */
static uint8_t  *omega_arr = NULL;/* Ω(i) for i up to MAX_N+4 */

#ifdef __cplusplus
#    include <vector>
static std::vector<seq_t> terms_vec;
#else
static seq_t *terms = NULL;       /* discovered sequence elements */
static size_t terms_cnt = 0;
static size_t terms_cap = 0;
#endif
static bool initialized = false;

/* Build spf and omega arrays up to MAX_N+4, then populate 'terms'. */
static void init_sequence(void)
{
    const size_t N = (size_t)MAX_N + 4;   /* need indices up to k+4 */

    /* allocate spf – 32‑bit is enough for numbers ≤ 2·10^7 */
    spf = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!spf) return;

    /* allocate omega array (uint8_t) */
    omega_arr = (uint8_t *)malloc((N + 1) * sizeof(uint8_t));
    if (!omega_arr) {
        free(spf);  spf = NULL;
        return;
    }

    /* initialise spf[i] = i */
    for (size_t i = 0; i <= N; ++i) spf[i] = (uint32_t)i;

    /* sieve for smallest prime factor */
    for (uint32_t p = 2; (uint64_t)p * p <= N; ++p)
        if (spf[p] == p) {                   /* p is prime */
            for (uint64_t m = (uint64_t)p * p; m <= N; m += p)
                if (spf[(size_t)m] == (uint32_t)m)
                    spf[(size_t)m] = (uint32_t)p;
        }

    /* compute Ω(i) via spf */
    omega_arr[0] = omega_arr[1] = 0;
    for (size_t i = 2; i <= N; ++i)
        omega_arr[i] = omega_arr[i / spf[i]] + 1;

#ifdef __cplusplus
    terms_vec.clear();
    terms_vec.reserve(20000);              /* estimated initial capacity */
#else
    terms_cap = 20000;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) {
        free(spf);  free(omega_arr);
        spf = omega_arr = NULL;
        return;
    }
    terms_cnt = 0;
#endif

    /* find all k satisfying the condition, 2 ≤ k ≤ MAX_N */
    for (size_t k = 2; k <= MAX_N; ++k) {
        uint8_t sum = omega_arr[k + 1] + omega_arr[k + 2]
                    + omega_arr[k + 3] + omega_arr[k + 4];
        if (omega_arr[k] == sum) {
#ifdef __cplusplus
            terms_vec.push_back((seq_t)k);
#else
            if (terms_cnt == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) {
                    free(spf);  free(omega_arr); free(terms);
                    spf = omega_arr = terms = NULL;
                    return;
                }
                terms = tmp;
            }
            terms[terms_cnt++] = (seq_t)k;
#endif
        }
    }

    initialized = true;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */
    if (!initialized) init_sequence();
    if (!initialized) return (seq_t)0;      /* in case init failed */

#ifdef __cplusplus
    size_t count = terms_vec.size();
    if ((idx_t)count < n) return (seq_t)0;
    return terms_vec[(size_t)(n - 1)];
#else
    if (terms_cnt < (size_t)n) return (seq_t)0;
    return terms[(size_t)(n - 1)];
#endif
}
