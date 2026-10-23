/*
 * A022942  a(n) = a(n-2) + c(n-1) for n >= 3, a( ) increasing, given a(1)=1,
 *          a(2)=2; where c( ) is complement of a( ).
 * https://oeis.org/A022942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022942
   a(n) = a(n-2) + c(n-1)  for n ≥ 3
   a(1) = 1, a(2) = 2
   where c(k) is the k‑th positive integer not belonging to the set {a(1),a(2),…}   */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper: membership test in the prefix of a ---------- */
static bool contains(const seq_t *a, seq_t val, idx_t upto)
/* true ⇔ val appears among a[1..upto] (1‑based) */
{
    idx_t l = 1, r = upto;
    while (l <= r) {
        idx_t m = l + ((r - l) >> 1);
        seq_t mid = a[m];
        if (mid == val) return true;
        if (mid < val)   l = m + 1;
        else             r = m - 1;
    }
    return false;
}

/* ---------- Cached data ------------------------------------ */
static seq_t *a_cache = NULL;          /* array of a[1..computed_to] */
static size_t cache_cap = 0;            /* allocated size   */
static idx_t  computed_to = 0;         /* largest n for which a_cache[n] is defined */

/* next integer to be tested for belonging to the complement,
   and how many missing numbers have already been produced */
static seq_t next_candidate = 1;
static idx_t missing_seen   = 0;

/* ---------- Ensure cache capacity --------------------------- */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();     /* out of memory */
        a_cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- Initialization ----------------------------------- */
static void init_sequence(void)
{
    if (computed_to) return;          /* already initialized */

    /* allocate space for at least the first two terms */
    ensure_capacity(2);
    a_cache[1] = 1;
    a_cache[2] = 2;
    computed_to = 2;
}

/* ---------- Main function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n = 0 */

    init_sequence();

    if (n > computed_to) {
        /* grow cache to hold up to n */
        ensure_capacity(n);
        for (idx_t i = computed_to + 1; i <= n; ++i) {
            /* compute the (i-1)-th missing integer c */
            seq_t cand = next_candidate;
            idx_t cnt = missing_seen;
            while (cnt < i-1) {
                if (!contains(a_cache, cand, i-1)) {
                    ++cnt;                     /* found a new missing number */
                    if (cnt == i-1) break;     /* we have reached c(i-1) */
                }
                ++cand;                          /* try next integer */
            }
            seq_t c_val = cand;
            next_candidate = cand + 1;   /* advance for future calls */
            missing_seen = i-1;          /* we have produced i-1 missing numbers */

            /* a(i) = a(i-2) + c(i-1) */
            a_cache[i] = a_cache[i-2] + c_val;
        }
        computed_to = n;
    }
    return a_cache[n];
}
