/*
 * A107458  Expansion of g.f.: (1-x^2-x^3)/( (1+x)*(1-x-x^3) ).
 * https://oeis.org/A107458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4); Initial values: a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=0
 * Range:     u8 n <= 21, u16 n <= 35, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* --------- storage management ------------------------------------------ */
#ifdef __cplusplus
/* In C++ use a std::vector so objects are constructed correctly. */
static std::vector<seq_t> dp;

/* Ensure dp has at least need+1 elements. */
static void ensure_capacity(idx_t need) {
    if (dp.size() > need) return;
    dp.resize(need + 1);            /* default‑construct required objects   */
}

/* Initialise the vector with base values.  Called once. */
static void init_storage(void) {
    if (dp.empty()) {
        ensure_capacity(3);         /* indices 0..3 */
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)0;
        dp[2] = (seq_t)0;
        dp[3] = (seq_t)0;
    }
}
#else
/* In C use a raw array that is reallocated with realloc. */
static seq_t *dp      = NULL;   /* array of terms, dp[i] = a(i)                */
static idx_t  cap     = 0;      /* current allocated capacity (in elements)    */
#ifdef _WIN32
#define EXIT_ON_MALLOC_FAIL exit(1)
#else
#define EXIT_ON_MALLOC_FAIL exit(1)
#endif

/* Ensure the allocated array can hold indices up to 'need'. */
static void ensure_capacity(idx_t need) {
    if (need < cap) return;
    idx_t new_cap = (cap) ? (cap << 1) : 4;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
    if (!tmp) EXIT_ON_MALLOC_FAIL;
    dp = tmp;
    cap = new_cap;
}

/* Initialise the array with base values.  Called once. */
static void init_storage(void) {
    static int called = 0;
    if (!called) {
        ensure_capacity(3);          /* indices 0..3 */
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)0;
        dp[2] = (seq_t)0;
        dp[3] = (seq_t)0;
        called = 1;
    }
}
#endif
/* ----------------------------------------------------------------------- */

/* Memoised recurrence: a[n] = a[n-2] + a[n-3] + a[n-4]   (n>=4) */
seq_t a(idx_t n) {
    init_storage();

#ifdef __cplusplus
    /* C++ implementation: vector provides size information */
    if (dp.size() <= n) {
        dp.resize(n + 1);
        for (idx_t i = 4; i <= n; ++i) {
            dp[i] = dp[i-2] + dp[i-3] + dp[i-4];
        }
    }
    return dp[n];
#else
    /* C implementation: use static array dp and a cached size */
    static idx_t cached = 0;   /* number of terms already computed (dp[0..cached-1]) */
    if (cached == 0) {
        cached = 4;            /* init_storage has set indices 0..3 */
    }
    if (cached <= n) {
        ensure_capacity(n);
        for (idx_t i = cached; i <= n; ++i) {
            /* i >= 4 because cached is at least 4 */
            dp[i] = dp[i-2] + dp[i-3] + dp[i-4];
        }
        cached = n + 1;
    }
    return dp[n];
#endif
}
