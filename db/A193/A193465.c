/*
 * A193465  Row sums of triangle A061312.
 * https://oeis.org/A193465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+1)! - A000166(n+2) (derangements)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses only unsigned arithmetic.  */

#include <stdlib.h>

/* Internal caches ----------------------------------------------------- */
static seq_t *fact_arr = NULL;   /* fact_arr[i] = i!                 */
static size_t fact_cap = 0;      /* capacity of fact_arr             */

static seq_t *der_arr  = NULL;   /* der_arr[i] = D(i)                */
static size_t der_cap  = 0;      /* capacity of der_arr              */

static seq_t *a_cache  = NULL;   /* a_cache[i] = a(i)                */
static size_t a_cap    = 0;      /* capacity of a_cache              */

static size_t fact_len = 0;      /* number of factorial entries      */
static size_t der_len  = 0;      /* number of derangement entries    */
static size_t a_len    = 0;      /* number of a entries              */

/* Ensure the three arrays contain data up to index N (inclusive).   */
static void ensure_up_to(idx_t N)
{
    /* Convert N to a size_t for array indices.  May truncate if very
       large, but the test harness only uses values small enough.
       The cast is safe because the loop bounds are < 2^64.   */
    size_t n = (size_t)N;

    /* Required sizes: factorial up to (n+1)!, derangements up to D(n+2),
       a(n) up to n.                                           */
    size_t need_fact = n + 2;   /* factorial indices 0 .. n+1 inclusive   */
    size_t need_der  = n + 3;   /* derangement indices 0 .. n+2 inclusive*/
    size_t need_a    = n + 1;   /* a indices 0 .. n inclusive             */

    /* Growth helper.  Double the capacity until it fits the need.   */
    #define GROW(cap, need, ptr)                                 \
        do {                                                     \
            if ((cap) < (need)) {                                \
                size_t new_cap = (cap) ? (cap) << 1 : 8;         \
                while (new_cap < (need)) new_cap <<= 1;          \
                ptr = (seq_t*)realloc(ptr, new_cap * sizeof(seq_t)); \
                cap = new_cap;                                   \
            }                                                    \
        } while (0)

    GROW(fact_cap, need_fact, fact_arr);
    GROW(der_cap,  need_der,  der_arr);
    GROW(a_cap,    need_a,    a_cache);
    #undef GROW

    /* Build factorials if needed. */
    if (fact_len == 0) {
        fact_arr[0] = (seq_t)1;
        fact_len = 1;
    }
    for (size_t i = fact_len; i < need_fact; ++i) {
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    }
    fact_len = need_fact;

    /* Build derangements if needed. */
    if (der_len == 0) {
        der_arr[0] = (seq_t)1; /* D(0) = 1 */
        der_arr[1] = (seq_t)0; /* D(1) = 0 */
        der_len = 2;
    }
    for (size_t i = der_len; i < need_der; ++i) {
        /* D(i) = (i-1) * (D(i-1) + D(i-2)) */
        seq_t sum = der_arr[i - 1] + der_arr[i - 2];
        der_arr[i] = (seq_t)(i - 1) * sum;
    }
    der_len = need_der;

    /* Compute a(n) values if needed. */
    for (size_t i = a_len; i < need_a; ++i) {
        /* a(i) = (i+1)*(i+1)! - D(i+2) */
        seq_t part  = fact_arr[i + 1] * (seq_t)(i + 1);
        a_cache[i] = part - der_arr[i + 2];
    }
    a_len = need_a;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_cache[(size_t)n];
}
