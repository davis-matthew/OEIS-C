/*
 * A141717  Numbers k such that the set of prime factors of phi(k) is a proper
 *          subset of the set of prime factors of k and the set of prime
 *          factors of k is a proper subset of the set of prime factors of
 *          sigma(k).
 * https://oeis.org/A141717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th integer k whose prime factors satisfy P(phi(k))
 *            ⊂ P(k) ⊂ P(sigma(k)). The implementation builds a small sieve of
 *            smallest prime factors up to 2·MAX_K, then scans integers
 *            increasing from 2 upward, collecting those that meet the
 *            condition. The results are cached, so subsequent cal...
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- constants ------------------------------------------------------- */
#define MAX_K (4000000ULL)            /* upper bound for k to generate */
#define MAX_SPF (MAX_K*2 + 10)        /* ensures all needed t's are factorable */

/* --- global sieve ---------------------------------------------- */
static uint32_t *spf = NULL;          /* smallest prime factor table */
static bool sieve_init = false;

/* simple allocation helpers ------------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 1024 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ----------------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve_init) return;
    spf = (uint32_t *)malloc((size_t)(MAX_SPF + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (uint64_t i = 0; i <= (uint64_t)MAX_SPF; ++i) spf[i] = (uint32_t)i;
    for (uint64_t i = 2; i * i <= (uint64_t)MAX_SPF; ++i) {
        if (spf[i] == (uint32_t)i) {                         /* i is prime */
            for (uint64_t j = i * i; j <= (uint64_t)MAX_SPF; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
    sieve_init = true;
}

/* factor distinct primes of n into array 'pr' (size ≤ 32) */
static int factor_distinct(uint64_t n, uint32_t pr[32])
{
    int sz = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        pr[sz++] = p;
        while (n % p == 0) n /= p;
    }
    return sz;
}

/* check whether k satisfies the subset conditions                 */
static bool is_candidate(uint64_t k)
{
    /* distinct primes of k */
    uint32_t pk[32];
    int pk_sz = factor_distinct(k, pk);

    /* phi(k) = k * Π (p-1)/p */
    uint64_t phi = k;
    for (int i = 0; i < pk_sz; ++i) {
        uint32_t p = pk[i];
        phi = phi / p * (p - 1);
    }
    /* distinct primes of phi */
    uint32_t pp[32];
    int pp_sz = factor_distinct(phi, pp);

    /* distinct primes of sigma(k)  (computed via prime power sums) */
    uint32_t sigma_tmp[128];
    int sigma_tmp_sz = 0;
    uint64_t m = k;
    while (m > 1) {
        uint32_t p = spf[m];
        int exp = 0;
        while (m % p == 0) { m /= p; exp++; }

        /* compute 1 + p + ... + p^exp */
        uint64_t t = 1;
        uint64_t p_pow = 1;
        for (int e = 0; e < exp; ++e) {
            p_pow *= p;
            t += p_pow;
        }
        /* add primes of t */
        uint32_t tmp[32];
        int tmp_sz = factor_distinct(t, tmp);
        for (int i = 0; i < tmp_sz; ++i)
            sigma_tmp[sigma_tmp_sz++] = tmp[i];
    }
    /* deduplicate sigma_tmp to sigma_sigma */
    if (sigma_tmp_sz == 0) sigma_tmp_sz = 0;
    /* simple insertion sort for small array */
    for (int i = 1; i < sigma_tmp_sz; ++i) {
        uint32_t key = sigma_tmp[i];
        int j = i - 1;
        while (j >= 0 && sigma_tmp[j] > key) {
            sigma_tmp[j + 1] = sigma_tmp[j];
            j--;
        }
        sigma_tmp[j + 1] = key;
    }
    uint32_t sigma_ps[32];
    int sigma_ps_sz = 0;
    for (int i = 0; i < sigma_tmp_sz; ++i) {
        if (i == 0 || sigma_tmp[i] != sigma_tmp[i - 1])
            sigma_ps[sigma_ps_sz++] = sigma_tmp[i];
    }

    /* check pp ⊂ pk (proper) */
    if (pp_sz >= pk_sz) return false;
    int i = 0, j = 0;
    while (i < pp_sz && j < pk_sz) {
        if (pp[i] == pk[j]) { ++i; ++j; }
        else if (pp[i] > pk[j]) ++j;
        else return false;
    }
    if (i < pp_sz) return false;

    /* check pk ⊂ sigma_ps (proper) */
    if (pk_sz >= sigma_ps_sz) return false;
    i = 0; j = 0;
    while (i < pk_sz && j < sigma_ps_sz) {
        if (pk[i] == sigma_ps[j]) { ++i; ++j; }
        else if (pk[i] > sigma_ps[j]) ++j;
        else return false;
    }
    if (i < pk_sz) return false;

    return true;
}

/* ----------------- public function ----------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cand = NULL;
    static size_t cand_len = 0, cand_cap = 0;
    static uint64_t last_scanned = 1;

    init_sieve();

    while (cand_len < n) {
        ++last_scanned;
        if (last_scanned > MAX_K) abort();        /* safety guard */

        if (is_candidate(last_scanned)) {
            grow_seq(&cand, &cand_cap, cand_len + 1);
            cand[cand_len++] = (seq_t)last_scanned;
        }
    }
    return cand[n - 1];
}
