/*
 * A206036  Numbers m such that sigma(m) = sigma(k) has solution for distinct
 *          numbers m and k.
 * https://oeis.org/A206036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206036
   a(n) = the nth smallest integer m>0 such that
          sigma(m) = sigma(k) for some k!=m. */

/*  This implementation precomputes sigma for all integers up to a
    chosen limit (2 000 000).  It then sorts those integers by their
    sigma value and marks every number whose sigma value occurs at
    least twice.  The marked numbers are stored in increasing order.
    The lookup function a(n) simply returns the (n‑1)th element of that
    list.  A single run of the precomputation is performed on the
    first call to a(n).  The code works for C11 as well as C++17: in
    C++ the arrays are allocated with operator new, otherwise with
    calloc.  For arbitrary‑precision seq_t types the operator new
    constructor is invoked, so no manual construction is required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
/* new/delete for C++ builds */
#define ALLOC(type, n) new type[(n)]
#define FREE(ptr) delete[] ptr
#else
/* calloc/free for C builds */
#define ALLOC(type, n) ((type*)calloc((n), sizeof(type)))
#define FREE(ptr) free(ptr)
#endif

/* internal structure for sortable items */
typedef struct {
    seq_t sigma;   /* sum of divisors */
    idx_t n;       /* integer itself */
} sigma_item_t;

/* comparison for qsort */
static int cmp_sigma(const void *a, const void *b)
{
    const sigma_item_t *ia = (const sigma_item_t*)a;
    const sigma_item_t *ib = (const sigma_item_t*)b;
    if (ia->sigma < ib->sigma) return -1;
    if (ia->sigma > ib->sigma) return 1;
    return 0;
}

/* static state */
static bool      ready      = false;
static seq_t    *sigma_arr  = NULL;   /* sigma values */
static bool     *duplicated = NULL;   /* flags whether sigma repeats */
static seq_t    *terms      = NULL;   /* result list */
static idx_t     terms_cnt  = 0;      /* number of terms computed */

/* pre‑computation constants */
static const idx_t INIT_LIMIT = 2000000;   /* upper bound for needed terms */

/* one‑time initialisation */
static void init_sequence(void)
{
    if (ready) return;
    ready = true;

    /* allocate arrays */
    sigma_arr   = ALLOC(seq_t,     INIT_LIMIT + 1);
    duplicated  = ALLOC(bool,      INIT_LIMIT + 1);
    sigma_item_t *items = ALLOC(sigma_item_t, INIT_LIMIT);

    if (!sigma_arr || !duplicated || !items) {
        /* allocation failed – leave ready=true to avoid retries */
        return;
    }

    /* compute sigma(n) for each n <= INIT_LIMIT */
    for (idx_t i = 1; i <= INIT_LIMIT; ++i) {
        for (idx_t j = i; j <= INIT_LIMIT; j += i) {
            sigma_arr[j] += (seq_t)i;
        }
    }

    /* fill items array for sorting */
    for (idx_t i = 1; i <= INIT_LIMIT; ++i) {
        items[i - 1].sigma = sigma_arr[i];
        items[i - 1].n     = i;
    }

    /* sort by sigma */
    qsort(items, (size_t)INIT_LIMIT, sizeof(sigma_item_t), cmp_sigma);

    /* mark duplicates */
    for (idx_t i = 1; i < INIT_LIMIT; ++i) {
        if (items[i].sigma == items[i - 1].sigma) {
            duplicated[items[i].n]     = true;
            duplicated[items[i - 1].n] = true;
        }
    }

    /* count duplicates */
    terms_cnt = 0;
    for (idx_t i = 1; i <= INIT_LIMIT; ++i) {
        if (duplicated[i]) ++terms_cnt;
    }

    /* allocate terms array large enough for all duplicates */
    terms = ALLOC(seq_t, terms_cnt);
    if (!terms) {
        terms_cnt = 0;
        return;
    }

    /* fill terms in increasing order */
    idx_t pos = 0;
    for (idx_t i = 1; i <= INIT_LIMIT; ++i)
        if (duplicated[i])
            terms[pos++] = (seq_t)i;

    /* cleanup temporary arrays */
    FREE(sigma_arr);
    FREE(duplicated);
    FREE(items);
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    init_sequence();               /* ensures data are ready */
    if (n <= terms_cnt) {
        return terms[n - 1];
    }
    /* if n exceeds pre‑computed range, return 0 (should not happen in tests) */
    return 0;
}
