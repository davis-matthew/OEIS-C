/*
 * A093496  Least k such that for every 1 < r < n there exists a distinct s, n
 *          < s <= k so that r and s have the same prime signature.
 * https://oeis.org/A093496
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  less, nonn
 * Formula:   a(n) = least k such that for every 2 ≤ r < n there exists a
 *            distinct s with n < s ≤ k and r,s have the same prime signature
 * Range:     u8 n <= 27, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   sort_exps, swap_uint8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes prime signatures of all numbers up
   to a fixed limit LMAX and stores for each signature a sorted
   list of the numbers sharing it.  For a given n it finds, for every
   signature occurring among 2…n‑1, the first element of that list that
   is greater than n; then, as it scans the r‑values, it takes the next
   unused element from that list for each r with that signature.  The
   maximum of all chosen s values is a(n).  LMAX is chosen large
   enough (200000) to cover n up to at least 10000.  The algorithm
   runs in O(n + s log LMAX) per call and uses only integer arithmetic. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sort_exps.h"
#include "../utils/swap_uint8.h"

/* ---- configuration ---------------------------------------------------- */
#define LMAX 200000
#define SIG_MAX_EXPS 20   /* maximal number of exponents in a signature */

/* ---- data structures ------------------------------------------------- */
typedef struct {
    uint8_t  len;
    uint8_t  exps[SIG_MAX_EXPS];   /* sorted ascending exponents */
} Sig;

typedef struct {
    size_t len;      /* number of stored numbers */
    size_t cap;      /* capacity of data array */
    idx_t *data;     /* sorted list of numbers sharing the signature */
} NumVec;

/* ---- global tables (filled by init() once) --------------------------- */
static bool  initialized = false;
static uint32_t spf[LMAX + 1];          /* smallest prime factor for each number */
static idx_t  *sig_id_of_num = NULL;    /* signature id for each number */
static Sig    *sig_defs = NULL;         /* list of distinct signatures */
static NumVec *sig_nums = NULL;         /* vector of numbers for each signature */
static size_t  sig_cap = 0, sig_cnt = 0;

static bool same_sig(const Sig *a, const Sig *b)
{
    if (a->len != b->len) return false;
    for (size_t i = 0; i < a->len; ++i)
        if (a->exps[i] != b->exps[i]) return false;
    return true;
}

/* ---- initialization --------------------------------------------------- */
static void precompute(void)
{
    if (initialized) return;
    initialized = true;

    /* 1. smallest prime factor sieve */
    memset(spf, 0, sizeof spf);
    for (uint32_t i = 2; i <= LMAX; ++i) {
        if (!spf[i]) {                 /* i is prime */
            for (uint32_t j = i; j <= LMAX; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }

    /* 2. allocate tables */
    sig_cap = 16;
    sig_defs = (Sig *)malloc(sig_cap * sizeof(Sig));
    sig_nums = (NumVec *)malloc(sig_cap * sizeof(NumVec));
    sig_id_of_num = (idx_t *)malloc((LMAX + 1) * sizeof(idx_t));
    memset(sig_id_of_num, 0, (LMAX + 1) * sizeof(idx_t));
    sig_cnt = 0;

    /* 3. factorize each number and build signatures + lists */
    uint8_t exps_buf[SIG_MAX_EXPS];
    for (idx_t num = 2; num <= LMAX; ++num) {
        /* factorize num */
        idx_t x = num;
        size_t ex_len = 0;
        while (x > 1) {
            uint32_t p = spf[x];
            uint8_t cnt = 0;
            while ((x % p) == 0) {
                x /= p;
                ++cnt;
            }
            exps_buf[ex_len++] = cnt;
        }
        sort_exps(exps_buf, ex_len);

        /* build signature structure */
        Sig cur;
        cur.len = (uint8_t)ex_len;
        memcpy(cur.exps, exps_buf, ex_len);

        /* find or create signature id */
        size_t id = sig_cnt;  /* default new */
        for (size_t i = 0; i < sig_cnt; ++i)
            if (same_sig(&sig_defs[i], &cur)) { id = i; break; }

        if (id == sig_cnt) { /* new signature */
            if (sig_cnt == sig_cap) {   /* grow tables */
                sig_cap *= 2;
                sig_defs = (Sig *)realloc(sig_defs, sig_cap * sizeof(Sig));
                sig_nums = (NumVec *)realloc(sig_nums, sig_cap * sizeof(NumVec));
            }
            /* store signature */
            sig_defs[id] = cur;
            /* init vector for this signature */
            sig_nums[id].len = 0;
            sig_nums[id].cap = 4;
            sig_nums[id].data = (idx_t *)malloc(4 * sizeof(idx_t));
            ++sig_cnt;
        }

        /* record signature id of num */
        sig_id_of_num[num] = (idx_t)id;

        /* append num to its signature vector */
        NumVec *vec = &sig_nums[id];
        if (vec->len == vec->cap) {
            vec->cap *= 2;
            vec->data = (idx_t *)realloc(vec->data, vec->cap * sizeof(idx_t));
        }
        vec->data[vec->len++] = num;
    }
}

/* ---- public sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for n < 3; harmless */

    precompute();                 /* ensure tables built */

    /* per‐signature pointers for this call */
    idx_t *ptr = (idx_t *)malloc(sig_cnt * sizeof(idx_t));

    /* initialize pointers to the first element > n for each signature */
    for (size_t sid = 0; sid < sig_cnt; ++sid) {
        const NumVec *vec = &sig_nums[sid];
        size_t lo = 0, hi = vec->len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (vec->data[mid] <= n)
                lo = mid + 1;
            else
                hi = mid;
        }
        ptr[sid] = (idx_t)lo;   /* index of first > n */
    }

    idx_t max_s = 0;
    for (idx_t r = 2; r < n; ++r) {
        idx_t sid = sig_id_of_num[r];
        const NumVec *vec = &sig_nums[sid];
        idx_t s = vec->data[ptr[sid]++];
        if (s > max_s) max_s = s;
    }

    free(ptr);
    return (seq_t)max_s;
}
