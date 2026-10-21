/*
 * A086494  Group the natural numbers such that the sum of the terms of every
 *          group has a distinct prime signature not occurring earlier: (1),
 *          (2), (3, 4, 5), (6), (7, 8, 9), (10, 11, 12, 13, 14), (15, 16,
 *          17), (18, 19, 20, 21)... Sequence contains the sum of the terms of
 *          groups.
 * https://oeis.org/A086494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the first natural numbers that form a new
 *            group. A new group starts whenever the sum of the numbers in the
 *            group has a prime‑signature (the multiset of exponents in its
 *            prime factorisation) that has not appeared for any earlier group
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------------------------------------------- */
/*  Prime table (sieve up to 1 000 000)                            */
#define PRIME_SIEVE_LIMIT 1000000
#define MAX_PRIMES 78498          // number of primes <= 1e6

static uint32_t primes[MAX_PRIMES];
static int prime_cnt = 0;

/* initialise the prime table on first use */
static void init_primes(void)
{
    if (prime_cnt != 0) return;
    bool *sieve = (bool *)malloc(PRIME_SIEVE_LIMIT + 1);
    memset(sieve, 1, PRIME_SIEVE_LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (int i = 2; (long long)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (long long j = (long long)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) primes[prime_cnt++] = i;
    free(sieve);
}

/* -------------------------------------------------------------- */
/*  Factorisation: returns sorted exponents of the prime factorisation */
static void factor_exponents(uint64_t n, int *res, int *len)
{
    int l = 0;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            int e = 0;
            while (n % primes[i] == 0) { n /= primes[i]; ++e; }
            res[l++] = e;
        }
    }
    if (n > 1) res[l++] = 1;   /* remaining prime factor */

    /* sort exponents increasingly to obtain a canonical signature */
    if (l > 1) {
        for (int i = 0; i < l - 1; ++i)
            for (int j = i + 1; j < l; ++j)
                if (res[i] > res[j]) {
                    int t = res[i]; res[i] = res[j]; res[j] = t;
                }
    }
    *len = l;
}

/* -------------------------------------------------------------- */
/*  Hash table of seen signatures                                 */
#define SIG_TAB_SIZE 200003    /* prime number > 2 * expected #signatures */

typedef struct {
    bool used;
    int len;          /* number of exponents in the signature */
    int *exps;        /* sorted vector of exponents (NULL if len==0) */
} SigEntry;

static SigEntry sig_table[SIG_TAB_SIZE];

/*  True if the signature of 'sum' is new and has been inserted */
static bool add_signature_if_new(uint64_t sum)
{
    int exps_local[64];
    int len;
    factor_exponents(sum, exps_local, &len);

    /* hash = length * 131^0 + exponent[0] * 131^1 + ... */
    uint64_t h = len;
    for (int i = 0; i < len; ++i) h = h * 1315423911ULL + (uint64_t)exps_local[i];
    size_t idx = h % SIG_TAB_SIZE;

    while (sig_table[idx].used) {
        if (sig_table[idx].len == len) {
            bool eq = true;
            for (int i = 0; i < len; ++i)
                if (sig_table[idx].exps[i] != exps_local[i]) { eq = false; break; }
            if (eq) return false;  /* already seen */
        }
        idx = (idx + 1) % SIG_TAB_SIZE;
    }

    /* insert new signature */
    sig_table[idx].used = true;
    sig_table[idx].len  = len;
    if (len > 0) {
        int *copy = (int *)malloc(len * sizeof(int));
        memcpy(copy, exps_local, len * sizeof(int));
        sig_table[idx].exps = copy;
    } else
        sig_table[idx].exps = NULL;
    return true;
}

/* -------------------------------------------------------------- */
/*  Main sequence array and generation                           */
static seq_t *group_sums = NULL;
static idx_t  groups_cnt = 0;
static idx_t  next_num   = 1;   /* next natural number to be added */

static void ensure_capacity(idx_t *idx, seq_t **arr, size_t *cap)
{
    if (*idx >= *cap) {
        size_t new_cap = (*cap == 0) ? 1024 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise helpers lazily */
    static bool primes_inited = false, sig_inited = false;
    if (!primes_inited) { init_primes(); primes_inited = true; }
    if (!sig_inited)   { memset(sig_table, 0, sizeof(sig_table)); sig_inited = true; }

    static size_t sig_cap = 0;
    static size_t grp_cap = 0;

    if (group_sums == NULL) {
        grp_cap = 0;
        group_sums = NULL;
    }

    while (groups_cnt <= n) {
        uint64_t sum = 0;
        while (true) {
            sum += (uint64_t)next_num;
            ++next_num;
            if (add_signature_if_new(sum)) {
                /* new group found */
                ensure_capacity(&groups_cnt, &group_sums, &grp_cap);
                group_sums[groups_cnt] = (seq_t)sum;
                ++groups_cnt;
                break;
            }
        }
    }
    return group_sums[n];
}
