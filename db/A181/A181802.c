/*
 * A181802  Triangle read by rows: T(n,k) is k-th smallest divisor of n that
 *          is highly composite (A002182).
 * https://oeis.org/A181802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A181802 – triangular read of highly composite divisors.
   Each n-th term is taken from the concatenation of rows, where
   row r lists the k‑th smallest divisor of r that is a highly
   composite number (A002182).  The function builds rows on demand
   and caches all results for fast random access. */

#include <stdlib.h>
#include <stddef.h>

/* ---------- compiler detection ---------- */
#ifdef __cplusplus
  #include <cstddef>
#else
  #include <stdint.h>
#endif

/* ---------- static state ---------- */
static idx_t      last_row   = 0;          /* last row that has been processed   */
static idx_t      total_terms = 0;         /* number of terms stored             */
static seq_t     *terms      = NULL;       /* concatenated rows (cumulative)     */
static size_t     terms_cap  = 0;          /* capacity of terms array            */
static size_t     terms_len  = 0;          /* number of elements in terms array   */

static size_t     hc_len     = 0;          /* number of very highly composite    */
static size_t     hc_cap     = 0;          /* capacity of hc array               */
static uint64_t  *hc         = NULL;       /* list of highly composite numbers   */

static idx_t      max_tau    = 0;          /* maximum divisor count so far       */

/* ---------- utility: ensure capacity for seq_t array ---------- */
static void
ensure_terms_cap(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    if (need > new_cap)
        new_cap = need;
#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < terms_len; ++i)
        new_arr[i] = terms[i];
    delete[] terms;
    terms = new_arr;
#else
    seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_arr;
#endif
    terms_cap = new_cap;
}

/* ---------- utility: ensure capacity for uint64_t array ---------- */
static void
ensure_hc_cap(size_t need)
{
    if (need <= hc_cap)
        return;
    size_t new_cap = hc_cap ? hc_cap * 2 : 8;
    if (need > new_cap)
        new_cap = need;
#ifdef __cplusplus
    uint64_t *new_arr = new uint64_t[new_cap];
    for (size_t i = 0; i < hc_len; ++i)
        new_arr[i] = hc[i];
    delete[] hc;
    hc = new_arr;
#else
    uint64_t *new_arr = (uint64_t*)realloc(hc, new_cap * sizeof(uint64_t));
    hc = new_arr;
#endif
    hc_cap = new_cap;
}

/* ---------- divisor count (trial division) ---------- */
static idx_t
divisor_count(idx_t n)
{
    idx_t res   = 1;
    idx_t value = n;
    for (idx_t p = 2; p * p <= value; ++p) {
        if (value % p == 0) {
            size_t e = 0;
            while (value % p == 0) {
                value /= p;
                ++e;
            }
            res *= (e + 1);
        }
    }
    if (value > 1)
        res *= 2;
    return res;
}

/* ---------- build next row and append its elements ---------- */
static void
build_next_row(void)
{
    idx_t r = last_row + 1;

    /* update the list of highly composite numbers */
    idx_t tau = divisor_count(r);
    if (tau > max_tau) {
        max_tau = tau;
        ensure_hc_cap(hc_len + 1);
        hc[hc_len++] = (uint64_t) r;
    }

    /* add all highly composite divisors of r to the terms array */
    for (size_t i = 0; i < hc_len; ++i) {
        uint64_t h = hc[i];
        if (h > r)
            break;                       /* hc is sorted ascending */
        if (r % h == 0) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)h;
            ++total_terms;
        }
    }

    last_row = r;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* the offset is 1; n==0 is undefined but handled */
        return (seq_t)0;

    /* extend precomputation until we have at least n terms */
    while (total_terms < n)
        build_next_row();

    return terms[n - 1];   /* zero‑based index inside terms */
}
