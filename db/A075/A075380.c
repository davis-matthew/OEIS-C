/*
 * A075380  Rearrangement of natural numbers so that the sum of two
 *          neighboring terms is not squarefree.
 * https://oeis.org/A075380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest permutation of the natural numbers
 *            such that the sum of two neighboring terms has a squared prime
 *            factor (i.e. is not square‑free). The implementation uses a
 *            greedy algorithm: at each step it chooses the smallest unused
 *            integer that yields a sum with the previous term t...
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------  Helper tables  ---------- */

/* permutation array */
static seq_t *perm          = NULL;   /* the sequence itself           */
static size_t perm_size     = 0;       /* number of terms already known */
static size_t perm_cap      = 0;       /* current allocation capacity  */

/* used[i] == 1 if i already appears in the permutation */
static uint8_t *used        = NULL;
static size_t used_limit    = 0;

/* is_sqfree[i] == 1 if i is square‑free (i.e. has no squared prime divisor) */
static uint8_t *is_sqfree   = NULL;
static size_t sqfree_limit  = 0;

/* ----------  Sizing helpers  ---------- */

static void grow_perm(void)
{
    if (perm_cap == 0) perm_cap = 4;
    else              perm_cap <<= 1;
    perm = (seq_t *)realloc(perm, perm_cap * sizeof(seq_t));
}

static void ensure_used(size_t need)
{
    if (used_limit >= need) return;
    size_t new_limit = used_limit ? (used_limit << 1) : 8;
    if (new_limit < need)   new_limit = need;
    used = (uint8_t *)realloc(used, (new_limit + 1) * sizeof(uint8_t));
    memset(used + used_limit + 1, 0, (new_limit - used_limit) * sizeof(uint8_t));
    used_limit = new_limit;
}

static void build_sqfree(size_t limit)
{
    if (sqfree_limit >= limit) return;
    uint8_t *new_arr = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    /* initialise all entries as square‑free; 0 will not be used */
    for (size_t i = 0; i <= limit; ++i) new_arr[i] = 1;

    /* first mark all non‑prime numbers as composite */
    size_t mx = limit;
    uint8_t *comp = (uint8_t *)calloc(mx + 1, sizeof(uint8_t));
    for (size_t i = 2; i * i <= mx; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= mx; j += i) comp[j] = 1;
        }
    }

    /* for each prime, mark all multiples of its square as NOT square‑free */
    for (size_t p = 2; p * p <= mx; ++p) {
        if (!comp[p]) {
            size_t p2 = p * p;
            for (size_t j = p2; j <= mx; j += p2) new_arr[j] = 0;
        }
    }

    free(comp);
    free(is_sqfree);
    is_sqfree   = new_arr;
    sqfree_limit = limit;
}

static void ensure_sqfree(size_t need)
{
    if (sqfree_limit >= need) return;
    /* use a generous limit to reduce frequent rebuilds */
    size_t new_limit = need * 2;
    if (new_limit < 16) new_limit = 16;
    build_sqfree(new_limit);
}

/* ----------  Sequence generator  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but harmless */
    while (perm_size < (size_t)n) {
        /* ----- find the next term ----- */
        size_t cand;
        if (perm_size == 0) {
            /* first element is the smallest unused integer */
            cand = 1;
        } else {
            seq_t prev = perm[perm_size - 1];
            cand = 1;
            for (;;) {
                if (cand > used_limit) ensure_used(cand);
                if (used[cand]) { ++cand; continue; }

                seq_t sum = prev + cand;
                size_t sum_usize = (size_t)sum;
                if (sum_usize > sqfree_limit) ensure_sqfree(sum_usize);

                /* accept if the sum is NOT square‑free */
                if (!is_sqfree[sum_usize]) break;
                ++cand;
            }
        }

        /* ----- append the chosen candidate ----- */
        if (used_limit < cand) ensure_used(cand);
        used[cand] = 1;

        if (perm_cap == perm_size) grow_perm();
        perm[perm_size++] = (seq_t)cand;
    }
    return perm[(size_t)n - 1];
}
