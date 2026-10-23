/*
 * A162966  Union of 1 and nonsquarefree numbers (A013929).
 * https://oeis.org/A162966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Union of 1 and non‑squarefree numbers (A013929)
   a(1) = 1, a(2) = 4, a(3) = 8, a(4) = 9, …
   The function returns the n‑th element for n >= 1.
   The algorithm builds a sequence of integers that are either 1
   or have a repeated prime factor, using a small‑prime‑factor sieve
   and caching the generated terms.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache of previously computed sequence values */
static seq_t *seq  = NULL;   /* array of terms */
static size_t seq_cap = 0;   /* allocated capacity in seq_t units */
static idx_t  seq_len  = 0;  /* number of terms currently stored */

/* last integer examined when building the sequence */
static idx_t last_checked = 0;

/* smallest prime factor table */
static idx_t *spf  = NULL;   /* dynamic array indexed by integer value */
static idx_t  spf_limit = 0; /* maximum integer covered by spf */

/* grow seq array to hold at least need elements */
static void seq_grow(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap*2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* build a smallest‑prime‑factor sieve up to “limit” */
static void spf_build(idx_t limit)
{
    idx_t *tmp = (idx_t*)calloc(limit + 1, sizeof(idx_t));
    if (!tmp) return;                          /* out of memory – unlikely */
    tmp[0] = 0;
    tmp[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (tmp[i] == 0) {                     /* i is prime */
            tmp[i] = i;
            if (i > limit / i) continue;       /* avoid overflow in i*i */
            for (idx_t j = i * i; j <= limit; j += i)
                if (tmp[j] == 0) tmp[j] = i;
        }
    }
    free(spf);
    spf = tmp;
    spf_limit = limit;
}

/* ensure the spf table covers at least n */
static void spf_ensure(idx_t n)
{
    if (n <= spf_limit) return;
    idx_t new_limit = n;
    if (new_limit < 64) new_limit = 64;
    else while (new_limit < n) new_limit <<= 1;
    spf_build(new_limit);
}

/* test whether m is squarefree using the spf table */
static bool is_squarefree(idx_t m)
{
    if (m <= 1) return true;  /* 1 is regarded as squarefree */
    while (m > 1) {
        idx_t p = spf[m];
        if (p == 0) p = m;    /* safety */
        int cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
            if (cnt > 1) return false;    /* repeated prime factor found */
        }
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0 */
    /* extend cache until we have at least n terms */
    while (seq_len < n) {
        ++last_checked;                       /* examine next integer */
        spf_ensure(last_checked);             /* ensure spf ready */

        if (last_checked == 1 || !is_squarefree(last_checked)) {
            seq_grow(seq_len + 1);
            seq[seq_len] = (seq_t)last_checked;
            ++seq_len;
        }
    }
    return seq[n-1];
}
