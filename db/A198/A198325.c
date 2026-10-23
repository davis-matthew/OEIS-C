/*
 * A198325  Irregular triangle read by rows: T(n,k) is the number of directed
 *          paths of length k (k>=1) in the rooted tree having Matula-Goebel
 *          number n (n>=2).
 * https://oeis.org/A198325
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term (offset 2) obtained by reading, by rows,
 *            the triangle T(n,k) of directed paths of length k in the rooted
 *            tree with Matula–Goebel number n (n ≥ 2). T(n,k) is defined
 *            recursively: Let n > 1 and let the multiset of child numbers
 *            C(n) = {c₁,…,cₗ} be the indices of the primes in...
 * Range:     u8 n <= 116, u16 n <= 116, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define PRIME_LIMIT 2000000          /* enough for all n up to about 10⁴ */

static unsigned *prime_list  = NULL;   /* primes in order, 1‑based index */
static size_t   prime_cnt    = 0;
static unsigned *prime_index = NULL;   /* prime_index[p] = position (1‑based) */

/* ---------- Prime sieve ------------------------------------------------- */
static void build_primes(void)
{
    unsigned *is_prime = (unsigned *)malloc((PRIME_LIMIT + 1) * sizeof(unsigned));
    if (!is_prime) abort();
    for (unsigned i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (unsigned i = 2; (unsigned long long)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (unsigned j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;

    prime_cnt = 0;
    for (unsigned i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_list = (unsigned *)malloc(prime_cnt * sizeof(unsigned));
    if (!prime_list) abort();

    prime_index = (unsigned *)malloc((PRIME_LIMIT + 1) * sizeof(unsigned));
    if (!prime_index) abort();
    memset(prime_index, 0, (PRIME_LIMIT + 1) * sizeof(unsigned));

    size_t pos = 0;
    for (unsigned i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_list[pos]   = i;
            prime_index[i]    = (unsigned)(pos + 1);    /* 1‑based */
            ++pos;
        }
    }
    free(is_prime);
}

/* ---------- Cached vectors for each n -------------------------------- */
typedef struct {
    seq_t *s;             /* S[n][k], 0 … len,  S[n][0] = 1 */
    seq_t *t;             /* T[n][k], 0 … len,  T[n][0] = 0 */
    size_t len;           /* maximum k with non‑zero value */
} VecInfo;

static VecInfo *vec_cache = NULL;   /* vector cache indexed by n */
static size_t   cache_cap = 0;      /* capacity of vec_cache */

/* Ensure vec_cache has space for indices up to n (inclusive) */
static void ensure_cache(size_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;
    VecInfo *new_cache = (VecInfo *)realloc(vec_cache, new_cap * sizeof(VecInfo));
    if (!new_cache) abort();
    for (size_t i = cache_cap; i < new_cap; ++i) {
        new_cache[i].s   = NULL;
        new_cache[i].t   = NULL;
        new_cache[i].len = 0;
    }
    vec_cache = new_cache;
    cache_cap = new_cap;
}

/* Factorise n into child numbers corresponding to prime indices */
static void factor_into_children(uint64_t n, unsigned *childs, size_t *cnt)
{
    *cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt &&
         (unsigned long long)prime_list[i] * prime_list[i] <= tmp; ++i) {
        unsigned p = prime_list[i];
        if (tmp % p == 0) {
            unsigned idx = prime_index[p];      /* 1‑based index of prime */
            if (idx == 0) abort();             /* should not happen */
            size_t exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            for (size_t e = 0; e < exp; ++e) {
                childs[(*cnt)++] = idx;
            }
        }
    }
    if (tmp > 1) {                  /* remaining prime factor */
        if (tmp > PRIME_LIMIT) abort();
        unsigned idx = prime_index[(unsigned)tmp];
        if (idx == 0) abort();
        childs[(*cnt)++] = idx;
    }
}

/* Recursively build and memoise vectors for a given tree number n */
static VecInfo *build_vector(uint64_t n)
{
    if (n < cache_cap && vec_cache[n].s != NULL) return &vec_cache[n];
    ensure_cache(n);

    VecInfo *info = &vec_cache[n];
    if (n == 1) {                  /* leaf */
        info->len = 0;
        info->s   = (seq_t *)malloc(sizeof(seq_t));
        info->t   = (seq_t *)malloc(sizeof(seq_t));
        if (!info->s || !info->t) abort();
        info->s[0] = (seq_t)1;
        info->t[0] = (seq_t)0;
        return info;
    }

    unsigned childs[32];           /* enough for any n ≤ 10⁶ */
    size_t child_cnt;
    factor_into_children(n, childs, &child_cnt);

    /* build child vectors first */
    size_t max_child_len = 0;
    VecInfo *child_vec[32];
    for (size_t i = 0; i < child_cnt; ++i) {
        child_vec[i] = build_vector(childs[i]);
        if (child_vec[i]->len > max_child_len)
            max_child_len = child_vec[i]->len;
    }

    /* sum over children */
    seq_t *sum_s = (seq_t *)calloc(max_child_len + 1, sizeof(seq_t));
    seq_t *sum_t = (seq_t *)calloc(max_child_len + 1, sizeof(seq_t));
    if (!sum_s || !sum_t) abort();
    for (size_t i = 0; i < child_cnt; ++i) {
        VecInfo *c = child_vec[i];
        for (size_t k = 0; k <= c->len; ++k) {
            sum_s[k] += c->s[k];
            sum_t[k] += c->t[k];
        }
    }

    info->len = max_child_len + 1;
    info->s   = (seq_t *)malloc((info->len + 1) * sizeof(seq_t));
    info->t   = (seq_t *)malloc((info->len + 1) * sizeof(seq_t));
    if (!info->s || !info->t) abort();

    info->s[0] = (seq_t)1;
    info->t[0] = (seq_t)0;

    for (size_t k = 1; k <= info->len; ++k) {
        seq_t sum_s_k_1 = (k - 1 <= max_child_len) ? sum_s[k - 1] : 0;
        seq_t sum_t_k   = (k <= max_child_len)          ? sum_t[k]   : 0;
        info->s[k] = sum_s_k_1;
        info->t[k] = sum_t_k + sum_s_k_1;
    }

    free(sum_s);
    free(sum_t);
    return info;
}

/* ---------- Flattened sequence storage -------------------------------- */
static seq_t *flat_terms = NULL;
static size_t  flat_cap   = 0;
static size_t  flat_len   = 0;
static uint64_t next_row  = 2;            /* next tree number to process */

/* Append cnt values from vals to the global flat array */
static void append_to_flat(seq_t *vals, size_t cnt)
{
    while (flat_len + cnt > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap << 1 : 16;
        seq_t *new_arr = (seq_t *)realloc(flat_terms, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        flat_terms = new_arr;
        flat_cap   = new_cap;
    }
    memcpy(&flat_terms[flat_len], vals, cnt * sizeof(seq_t));
    flat_len += cnt;
}

/* ---------- Public entry ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!prime_list) build_primes();          /* initialise primes once */

    if (n < 2) return (seq_t)0;              /* undefined below offset */

    size_t need = (size_t)(n - 2);           /* index in the flattened array */

    while (flat_len <= need) {               /* generate rows until enough terms */
        VecInfo *row = build_vector(next_row);
        append_to_flat(row->t + 1, row->len); /* skip t[0] */
        ++next_row;
    }
    return flat_terms[need];
}
