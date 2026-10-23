/*
 * A153516  Triangle T(n,k,p,q,j) = T(n-1,k,p,q,j) + T(n-1,k-1,p,q,j) +
 *          (p*j+q)*prime(j)*T(n-2,k-1,p,q,j) with T(2,k,p,q,j) = prime(j) and
 *          (p,q,j) = (0,1,2), read by rows.
 * https://oeis.org/A153516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 241, u64 n <= 922,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) defined by
   T(n,k) = T(n-1,k)+T(n-1,k-1)+3*T(n-2,k-1)
   with T(1,1)=2, T(2,k)=3, T(3,2)=14, and T(n,1)=T(n,n)=2.
   Sequence a(n) is the triangle read row‑by‑row. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stores a(1), a(2), … */
static size_t cache_len = 0;         /* current number of entries in cache   */
static size_t cache_cap = 0;         /* allocated capacity of cache          */
static idx_t  rows_done = 0;         /* number of rows that have already been computed */

/* Ensure that cache has room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Push a value into the cache. */
static void push_to_cache(seq_t val)
{
    ensure_cache_capacity();
    cache[cache_len++] = val;
}

/* Compute rows of the triangle until we have at least index n (1‑based). */
static void compute_up_to_idx(idx_t n)
{
    if (n == 0)
        return;

    /* Determine the row number that contains position n. */
    idx_t target_row = 1;
    while (target_row * (target_row + 1) / 2 < n)
        ++target_row;

    while (rows_done < target_row) {
        ++rows_done;         /* current row number (1‑based) */
        idx_t r = rows_done; /* row length */

        if (r == 1) {                      /* row 1: 2 */
            push_to_cache((seq_t)2);
        } else if (r == 2) {                /* row 2: 3,3 */
            push_to_cache((seq_t)3);
            push_to_cache((seq_t)3);
        } else if (r == 3) {                /* row 3: 2,14,2 */
            push_to_cache((seq_t)2);
            push_to_cache((seq_t)14);
            push_to_cache((seq_t)2);
        } else {                            /* r >= 4 */
            /* leftmost element */
            push_to_cache((seq_t)2);

            /* pre‑compute starts of previous rows for indexing */
            idx_t start_nm1 = (r - 1) * (r - 2) / 2;   /* row r‑1 */
            idx_t start_nm2 = (r - 2) * (r - 3) / 2;   /* row r‑2 */

            for (idx_t k = 2; k <= r - 1; ++k) {
                seq_t a1 = cache[start_nm1 + (k - 1)];
                seq_t a2 = cache[start_nm1 + (k - 2)];
                seq_t a3 = cache[start_nm2 + (k - 2)];
                seq_t val = a1 + a2 + (seq_t)3 * a3;
                push_to_cache(val);
            }

            /* rightmost element */
            push_to_cache((seq_t)2);
        }
    }
}

/* Public entry point: a(n) – the nth term, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined by the OEIS offset */
    compute_up_to_idx(n);
    return cache[n - 1];
}
