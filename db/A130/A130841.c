/*
 * A130841  Number of ways to write n as a sum of oterms, where an oterm is an
 *          ordered product of (1+oterm), sorted by size and an empty product
 *          has value 1.
 * https://oeis.org/A130841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over sequences (n1,…,nk) with 2 ≤ n1 ≤ … ≤ nk, *
 *            n1⋯nk = n of ∏ a(nj−1) (a(1)=1)
 * Range:     u8 n <= 37, u16 n <= 149, u32 n <= 1039, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Custom types supplied by the build system:
 *   seq_t  – the type that holds the sequence values
 *   idx_t  – the type used for indices (unsigned 64‑bit)                 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration for use in the helper */
static seq_t factor_count(idx_t n, idx_t min_factor);

/* Memoised array of a(1)…a(max_n).  The array is grown on demand. */
static seq_t *a_arr = NULL;     /* pointer to the array */
static idx_t  arr_cap = 0;      /* allocated capacity (≥ index) */
static idx_t  computed = 0;     /* last index that has a computed value */

/* Allocate or enlarge the memo array to hold at least max_n+1 entries. */
static void ensure_capacity(idx_t max_n)
{
    if (max_n < 0) return;      /* should never happen for unsigned idx_t */

    if (arr_cap <= max_n) {
        /* grow by a factor of two for efficiency */
        idx_t new_cap = arr_cap ? arr_cap << 1 : 8;
        if (new_cap <= max_n) new_cap = max_n + 1;
        seq_t *new_arr = (seq_t *)realloc(a_arr, (size_t)new_cap * sizeof(seq_t));
        if (new_arr == NULL) {
            /* Allocation failed – terminate with undefined behaviour.
               The build system may abort the program in such a case. */
            abort();
        }
        a_arr = new_arr;
        arr_cap = new_cap;
    }
}

/* Recursive helper that counts weighted factorizations of `n`
 * with all factors ≥ `min_factor`.  The result is
 *    sum_{d | n, d ≥ min_factor} a(d-1) * factor_count(n/d, d)
 * plus the term for d = n (which yields an empty product for n/d = 1). */
static seq_t factor_count(idx_t n, idx_t min_factor)
{
    if (n == 1) return 1;   /* empty product */

    seq_t sum = 0;
    idx_t d;
    /* iterate over proper divisors up to sqrt(n) */
    for (d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d >= min_factor) {
                /* factor d followed by a factorization of n/d with min d */
                sum += a_arr[d - 1] * factor_count(n / d, d);
            }
            idx_t e = n / d;
            if (e != d && e >= min_factor) {
                sum += a_arr[e - 1] * factor_count(n / e, e);
            }
        }
    }
    /* case where we take the whole n as a factor */
    if (n >= min_factor) {
        sum += a_arr[n - 1];   /* factor n, then empty product for 1 */
    }
    return sum;
}

/* Public function requested by the specification. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* values below offset are undefined */

    /* Ensure memo table is large enough and compute missing values */
    if (n > computed) {
        if (computed == 0) {
            /* initialise a(1) = 1 */
            ensure_capacity(1);
            a_arr[1] = 1;
            computed = 1;
        }
        ensure_capacity(n);
        size_t i;
        for (i = computed + 1; i <= n; ++i) {
            a_arr[i] = factor_count(i, 2);
        }
        computed = n;
    }
    return a_arr[n];
}
