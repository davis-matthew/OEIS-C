/*
 * A051539  a(n) is the least common multiple of {1, 5, 9, 13, 17, ..., 4n+1}
 *          (A016813).
 * https://oeis.org/A051539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = lcm{1, 5, 9, 13, …, 4n+1} (OEIS A051539)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  internal state  ---------- */
static seq_t *lcm_vals = NULL;   /* a(n) values already computed */
static size_t  lcm_cap  = 0;     /* allocated capacity of lcm_vals   */
static size_t  lcm_len  = 0;     /* number of values stored (max n+1) */

/* prime exponent bookkeeping */
static idx_t  *prime_list = NULL;  /* list of distinct primes seen */
static idx_t  *prime_exp  = NULL;  /* current maximal exponent for each */
static size_t  prime_cnt  = 0;
static size_t  prime_cap  = 0;

/* current lcm value for the greatest n already processed */
static seq_t cur_lcm = 1;

/* ----------  helpers  ---------- */
static void ensure_lcm_cap(size_t need)
{
    if (lcm_cap < need) {
        size_t new_cap = lcm_cap ? lcm_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        lcm_vals = (seq_t *)realloc(lcm_vals, new_cap * sizeof(seq_t));
        lcm_cap = new_cap;
    }
}

static void ensure_prime_cap(void)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        prime_exp  = (idx_t *)realloc(prime_exp,  new_cap * sizeof(idx_t));
        prime_cap  = new_cap;
    }
}

/* add/update a prime factor to the running lcm */
static void update_prime(idx_t p, idx_t exp)
{
    /* find if p already present */
    size_t i;
    for (i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] == p) break;
    }

    if (i == prime_cnt) {            /* new prime */
        ensure_prime_cap();
        prime_list[prime_cnt] = p;
        prime_exp[prime_cnt]  = 0;
        ++prime_cnt;
    } else if (exp <= prime_exp[i]) { /* nothing to do */
        return;
    }

    /* multiply cur_lcm by p^(exp - current) */
    idx_t delta = exp - prime_exp[i];
    seq_t prod = cur_lcm;
    for (idx_t j = 0; j < delta; ++j) prod *= (seq_t)p;
    cur_lcm = prod;
    prime_exp[i] = exp;
}

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (lcm_vals == NULL) {
        ensure_lcm_cap(1);
        lcm_vals[0] = 1;      /* a(0) = 1 */
        lcm_len = 1;
    }

    /* if already computed, return immediately */
    if (n < lcm_len) return lcm_vals[n];

    /* extend computation up to n */
    for (idx_t k = lcm_len; k <= n; ++k) {
        uint64_t m = 4ULL * (uint64_t)k + 1;   /* term to add */

        /* factorise m by trial division */
        uint64_t temp = m;
        for (uint64_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) {
                idx_t e = 0;
                do {
                    temp /= d;
                    ++e;
                } while (temp % d == 0);
                update_prime((idx_t)d, e);
            }
        }
        if (temp > 1) {            /* remainder is prime */
            update_prime((idx_t)temp, 1);
        }

        /* store current lcm as a(k) */
        ensure_lcm_cap(k + 1);
        lcm_vals[k] = cur_lcm;
    }

    lcm_len = n + 1;
    return lcm_vals[n];
}
