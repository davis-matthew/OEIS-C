/*
 * A063967  Triangle read by rows, T(n,k) = T(n-1,k) + T(n-2,k) + T(n-1,k-1) +
 *          T(n-2,k-1) and T(0,0) = 1.
 * https://oeis.org/A063967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, changed
 * Formula:   a(n) comes from the triangle T(n,k) with recurrence T(n,k) =
 *            T(n-1,k) + T(n-2,k) + T(n-1,k-1) + T(n-2,k-1), T(0,0)=1, and
 *            a(n) is the nth entry of the flattened triangle read by rows
 * Range:     u8 n <= 30, u16 n <= 94, u32 n <= 331, u64 n <= 1143,
 *            u128 n <= 4220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- Common helper  ---------------------------------- */

static void grow_terms(
#ifdef __cplusplus
    std::vector<seq_t> &vec,
    size_t need
#else
    seq_t **arr,
    size_t *len,
    size_t *cap,
    size_t need
#endif
)
{
#ifdef __cplusplus
    if (need <= vec.capacity()) return;
    size_t newcap = vec.capacity() ? vec.capacity() * 2 : 64;
    while (newcap < need) newcap <<= 1;
    vec.reserve(newcap);
#else
    if (need <= *cap) return;
    size_t newcap = *cap ? *cap * 2 : 64;
    while (newcap < need) newcap <<= 1;
    seq_t *newptr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
    if (!newptr) abort();
    *arr = newptr;
    *cap = newcap;
#endif
}

/* ---------- C++ storage ------------------------------------- */

#ifdef __cplusplus

static struct {
    std::vector<seq_t> terms;    /* flattened triangle */
    std::vector<seq_t> prev;     /* row n-1   */
    std::vector<seq_t> prevprev; /* row n-2   */
    unsigned next_row_index = 1; /* next row to generate */
} cache;

static void ensure_a_upto(idx_t n)
{
    if (cache.terms.size() > n) return;          /* already ready */

    /* initialise first row if necessary */
    if (cache.terms.empty()) {
        cache.terms.reserve(64);
        cache.terms.push_back((seq_t)1);          /* T(0,0) */
        cache.prev.resize(1);
        cache.prev[0] = (seq_t)1;                 /* row0 = prev   */
        cache.prevprev.clear();                   /* no row -1     */
    }

    /* generate rows until enough entries */
    while (cache.terms.size() <= (size_t)n) {
        unsigned r = cache.next_row_index;        /* current row index */
        size_t new_len = r + 1;                   /* elements in this row */

        std::vector<seq_t> cur;
        cur.resize(new_len);

        for (size_t k = 0; k < new_len; ++k) {
            seq_t val = 0;
            if (k < cache.prev.size())      val += cache.prev[k];
            if (k < cache.prevprev.size())  val += cache.prevprev[k];
            if (k > 0) {
                if ((k-1) < cache.prev.size())     val += cache.prev[k-1];
                if ((k-1) < cache.prevprev.size()) val += cache.prevprev[k-1];
            }
            cur[k] = val;
        }

        /* append the row to the flattened list */
        cache.terms.insert(cache.terms.end(), cur.begin(), cur.end());

        /* rotate row buffers */
        cache.prevprev = std::move(cache.prev);
        cache.prev = std::move(cur);

        ++cache.next_row_index;
    }
}

seq_t a(idx_t n)
{
    ensure_a_upto(n);
    return cache.terms[(size_t)n];
}

#else /* ---------- C storage --------------------------------- */

/* C version uses raw dynamic arrays.  All allocations are cast
   to (seq_t*) and manual copying is done with memcpy/assignment.  */
static struct {
    seq_t *terms;
    size_t terms_len;
    size_t terms_cap;

    seq_t *prev;
    size_t len_prev;
    seq_t *prevprev;
    size_t len_prevprev;

    unsigned next_row_index;
} ccache = {0,0,0, NULL,0, NULL,0, 1};

static void ensure_a_upto(idx_t n)
{
    if (ccache.terms_len > (size_t)n) return;  /* ready */

    /* initialise first row if needed */
    if (ccache.terms_len == 0) {
        grow_terms(&ccache.terms, &ccache.terms_len, &ccache.terms_cap, 1);
        ccache.terms[0] = (seq_t)1;
        ccache.terms_len = 1;
        ccache.prev = (seq_t*)malloc(sizeof(seq_t));
        ccache.prev[0] = (seq_t)1;
        ccache.len_prev = 1;
    }

    /* generate rows until enough entries */
    while (ccache.terms_len <= (size_t)n) {
        unsigned r = ccache.next_row_index;
        size_t new_len = r + 1;

        seq_t *cur = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!cur) abort();

        for (size_t k = 0; k < new_len; ++k) {
            seq_t val = 0;
            if (k < ccache.len_prev)          val += ccache.prev[k];
            if (k < ccache.len_prevprev)      val += ccache.prevprev[k];
            if (k > 0) {
                if ((k-1) < ccache.len_prev)     val += ccache.prev[k-1];
                if ((k-1) < ccache.len_prevprev) val += ccache.prevprev[k-1];
            }
            cur[k] = val;
        }

        grow_terms(&ccache.terms, &ccache.terms_len, &ccache.terms_cap, ccache.terms_len + new_len);
        memcpy(ccache.terms + ccache.terms_len, cur, new_len * sizeof(seq_t));
        ccache.terms_len += new_len;

        /* rotate buffers */
        free(ccache.prevprev);
        ccache.prevprev = ccache.prev;
        ccache.len_prevprev = ccache.len_prev;
        ccache.prev = cur;
        ccache.len_prev = new_len;

        ++ccache.next_row_index;
    }
}

seq_t a(idx_t n)
{
    ensure_a_upto(n);
    return ccache.terms[(size_t)n];
}

#endif /* end C++ / C */
