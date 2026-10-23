/*
 * A023633  a(n) = c([ n/3 ]) + n, with a(1) = 1, a(2) = 2, c = complement to
 *          a.
 * https://oeis.org/A023633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(⌊n/3⌋) + n, where c is the complement of a (positive
 *            integers not in a)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic arrays for a(n) and its complement c(k) */
static seq_t *a_seq   = NULL;      /* a_seq[1..] holds the sequence values */
static seq_t *c_seq   = NULL;      /* c_seq[k] = k‑th unused positive integer */
static idx_t  max_a_n = 0;         /* largest n already computed */
static idx_t  max_c_k = 0;         /* largest k already computed */
static size_t a_cap   = 0;         /* current capacity of a_seq (number of elements allocated) */
static size_t c_cap   = 0;         /* current capacity of c_seq */

/* enlarge the a_seq array to at least 'cap' elements (0 .. cap-1) */
static void ensure_a_capacity(size_t cap) {
    if (cap <= a_cap) return;
    size_t new_cap = a_cap ? a_cap : 16;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *buf = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!buf) exit(EXIT_FAILURE);
    /* zero the new part so that binary search behaves correctly */
    if (max_a_n + 1 < new_cap)
        memset(buf + max_a_n + 1, 0, (new_cap - max_a_n - 1) * sizeof(seq_t));
    a_seq  = buf;
    a_cap  = new_cap;
}

/* enlarge the c_seq array to at least 'cap' elements (0 .. cap-1) */
static void ensure_c_capacity(size_t cap) {
    if (cap <= c_cap) return;
    size_t new_cap = c_cap ? c_cap : 16;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *buf = (seq_t *)realloc(c_seq, new_cap * sizeof(seq_t));
    if (!buf) exit(EXIT_FAILURE);
    /* zero the new part for safety */
    if (max_c_k + 1 < new_cap)
        memset(buf + max_c_k + 1, 0, (new_cap - max_c_k - 1) * sizeof(seq_t));
    c_seq  = buf;
    c_cap  = new_cap;
}

/* binary search: is 'val' present in a_seq[1..upto]? */
static int binary_search_a(seq_t val, idx_t upto) {
    idx_t lo = 1, hi = upto;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t midv = a_seq[mid];
        if (midv == val) return 1;
        if (midv < val)  lo = mid + 1;
        else             hi = mid - 1;
    }
    return 0;
}

/* compute all values up to 'target_n' (at least 1) */
static void compute_upto(idx_t target_n) {
    if (target_n <= max_a_n) return;          /* already done */

    /* initialisation */
    if (max_a_n == 0) {
        /* reserve minimal space */
        ensure_a_capacity(3);          /* indices 0..2 */
        ensure_c_capacity(2);          /* indices 0..1 */
        a_seq[1] = (seq_t)1;
        a_seq[2] = (seq_t)2;
        max_a_n = 2;

        /* first unused positive integer is 3 */
        c_seq[1] = (seq_t)3;
        max_c_k = 1;
    }

    /* make sure a_seq can hold up to target_n */
    ensure_a_capacity((size_t)(target_n + 1));

    /* generate terms sequentially */
    for (idx_t i = max_a_n + 1; i <= target_n; ++i) {
        idx_t k = i / 3;  /* floor(i/3) */

        /* generate complement up to k */
        while (max_c_k < k) {
            /* find next unused integer after last stored c */
            seq_t cand = c_seq[max_c_k] + (seq_t)1;
            while (binary_search_a(cand, max_a_n)) {
                cand = cand + (seq_t)1;
            }
            /* store it */
            ensure_c_capacity(max_c_k + 2);
            c_seq[max_c_k + 1] = cand;
            ++max_c_k;
        }

        /* compute a(i) = c(k) + i */
        a_seq[i] = c_seq[k] + (seq_t)i;
    }

    max_a_n = target_n;
}

/* public function required by the task */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    compute_upto(n);
    return a_seq[n];
}
