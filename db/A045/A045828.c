/*
 * A045828  One fourth of theta series of cubic lattice with respect to face.
 * https://oeis.org/A045828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A045828
 *
 *  The sequence is defined as the number of ordered triples of triangular
 *  numbers (t1, t2, t3) satisfying
 *
 *          t1 + t2 + 2*t3 = n.
 *
 *  Here a triangular number is T(k) = k*(k+1)/2, k >= 0.
 *
 *  The implementation computes the answer by enumerating t3 and, for
 *  each residue, counting all ordered pairs (t1, t2) that sum to the
 *  remaining value.  A small cache of triangular numbers is kept so
 *  that membership tests are performed via binary search.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Triangular number cache                                         */
static seq_t *tri = NULL;       /* array of triangular numbers */
static size_t tri_len = 0;      /* number of elements currently stored */
static size_t tri_cap = 0;      /* allocated capacity */

/*  Grow the triangular number cache to accommodate at least `len` elements  */
static void grow_tri_cache(size_t len)
{
    if (len <= tri_cap) return;
    size_t new_cap = tri_cap ? tri_cap * 2 : 8;
    while (new_cap < len) new_cap <<= 1;
    tri = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
    tri_cap = new_cap;
}

/*  Ensure that the cache contains all triangular numbers up to `max_val`.  */
static void ensure_tri_up_to(seq_t max_val)
{
    if (tri == NULL) {
        grow_tri_cache(8);
        tri[0] = 0;           /* T(0) */
        tri_len = 1;
    }
    while (tri[tri_len-1] <= max_val) {
        /* T(k) = T(k-1) + k, where k = tri_len */
        seq_t next = tri[tri_len-1] + (seq_t)tri_len;
        grow_tri_cache(tri_len + 1);
        tri[tri_len] = next;
        tri_len++;
    }
}

/*  Binary search: return 1 if `val` is a triangular number, else 0.   */
static int is_triangular(seq_t val)
{
    if (tri_len == 0 || tri[tri_len-1] < val) {
        ensure_tri_up_to(val);
    }
    size_t low = 0, high = tri_len - 1;
    while (low <= high) {
        size_t mid = (low + high) >> 1;
        seq_t v = tri[mid];
        if (v == val) return 1;
        if (v < val)  low = mid + 1;
        else          high = mid - 1;
    }
    return 0;
}

/*  Count ordered pairs (t1, t2) of triangular numbers with sum = `rest`.  */
static seq_t count_pairs(seq_t rest)
{
    seq_t cnt = 0;
    for (size_t a = 0; a < tri_len && tri[a] <= rest; ++a) {
        seq_t rem = rest - tri[a];
        if (is_triangular(rem)) cnt++;
    }
    return cnt;
}

/*  Main sequence function                                            */
seq_t a(idx_t n)
{
    /* The cache must handle values up to n for membership tests.   */
    ensure_tri_up_to((seq_t)n);

    seq_t total = 0;
    for (seq_t c = 0; ; ++c) {
        seq_t Tc = c * (c + 1);          /* 2 * T(c) = c*(c+1)        */
        if (Tc > n) break;
        seq_t rem = (seq_t)n - Tc;
        total += count_pairs(rem);
    }
    return total;
}
