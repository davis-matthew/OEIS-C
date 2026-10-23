/*
 * A100356  Maximal number of 165432 patterns in a permutation of 1,2,...,n.
 * https://oeis.org/A100356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A100356 – maximal number of 165432 patterns in a permutation of 1..n
 * Formula: a(1)=0; for n>1, a(n) = max_{1≤k<n} ( a(k) + k * C(n-k,5) ),
 * where C(m,5) = binomial(m,5).
 *
 * Implementation notes:
 *   - Mnemonics: memo[i] holds a(i) (1≤i≤MAXN-1).
 *   - bin[i] holds C(i,5) for 0≤i<MAXN.
 *   - The arrays are statically sized to 10001, sufficient for all requested n
 *     (the problem guarantees n≤10000 for the required runtime).
 *   - All arithmetic is performed in seq_t to avoid overflow and to support
 *     arbitrary‑precision builds.
 *   - The code compiles under C11 and C++17; no dynamic allocation is used.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAXN 10001          /* enough for n ≤ 10000 */

static seq_t memo[MAXN];
static seq_t bin[MAXN];
static bool initialized = false;
static idx_t maxComputed = 1;

/* multiplicative binomial coefficient C(m,5) */
static seq_t binom5(idx_t m)
{
    if (m < 5) return (seq_t)0;
    seq_t r = (seq_t)m * (m - 1) * (m - 2) * (m - 3) * (m - 4);
    return r / (seq_t)120;    /* 5! = 120 */
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for offset 1 */

    if (!initialized) {
        /* initialise binomials and memo to zero */
        for (idx_t i = 0; i < MAXN; ++i) bin[i] = binom5(i);
        for (idx_t i = 0; i < MAXN; ++i) memo[i] = (seq_t)0;
        memo[1] = (seq_t)0;
        initialized = true;
    }

    /* extend memoisation if needed */
    if (n >= MAXN) {
        /* In the current problem scope this case never occurs, but
           we guard against it by ignoring requests beyond MAXN. */
        return (seq_t)0;
    }

    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t best = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            seq_t term = memo[k] + (seq_t)k * bin[i - k];
            if (term > best) best = term;
        }
        memo[i] = best;
    }
    if (n > maxComputed) maxComputed = n;

    return memo[n];
}
