/*
 * A184707  Number of strings of numbers x(i=1..7) in 0..n with sum i*x(i)
 *          equal to n*7.
 * https://oeis.org/A184707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S⊆{1..7}} (-1)^|S| · p_{≤7}(7n – (n+1)·Σ_{i∈S} i)
 *            where p_{≤7}(t) is the number of solutions to Σ i·y(i)=t with
 *            y(i)≥0 (unbounded)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 79, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A184707 – Number of 7‑tuples 0≤x(i)≤n with Σ i·x(i)=7n  */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

/* DP table for the unbounded problem: number of ways to reach each sum. */
static seq_t *dp = NULL;      /* dp[t] holds p_{≤7}(t) for 0≤t≤dp_size-1 */
static size_t dp_size = 0;    /* number of entries actually allocated */

/* Pre‑computed data for each subset of the set {1,…,7}. */
static int  subset_sigma[128];
static unsigned char subset_bits[128];
static bool subsets_initialized = false;

/* Initialise subset tables (called once). */
static void init_subsets(void)
{
    if (subsets_initialized) return;
    for (int s = 0; s < 128; ++s) {
        int sigma = 0;
        unsigned char bits = 0;
        for (int i = 0; i < 7; ++i) {
            if (s & (1 << i)) {
                sigma += (i + 1);   /* weight i+1 */
                ++bits;
            }
        }
        subset_sigma[s] = sigma;
        subset_bits[s]  = bits;
    }
    subsets_initialized = true;
}

/* Ensure dp contains counts up to max_t (inclusive). */
static void ensure_dp(size_t max_t)
{
    if (dp && dp_size > max_t) return;  /* already sufficient */

    /* Re‑allocate new table; old one is discarded. */
    free(dp);
    /* In C/C++ both allow casting of void* from calloc. */
    dp = (seq_t *)calloc(max_t + 1, sizeof(seq_t));
    if (!dp) exit(1);          /* memory allocation failed */
    dp[0] = (seq_t)1;
    /* Classic unbounded coin change DP for coin denominations 1..7. */
    for (int w = 1; w <= 7; ++w) {
        for (size_t t = w; t <= max_t; ++t) {
            dp[t] += dp[t - w];
        }
    }
    dp_size = max_t + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – out of range */

    init_subsets();

    const size_t target_sum = 7 * (size_t)n;
    ensure_dp(target_sum);

    seq_t pos = 0, neg = 0;
    const int nplus1 = (int)n + 1;

    for (int mask = 0; mask < 128; ++mask) {
        int sigma = subset_sigma[mask];
        long long tmp = (long long)target_sum - (long long)nplus1 * sigma;
        if (tmp < 0) continue;          /* impossible target */

        seq_t ways = dp[(size_t)tmp];
        if (subset_bits[mask] & 1) {     /* odd subset size → sign − */
            neg += ways;
        } else {                         /* even subset size → sign + */
            pos += ways;
        }
    }

    /* pos ≥ neg for all valid n, but subtraction is safe in unsigned types. */
    return pos - neg;
}
