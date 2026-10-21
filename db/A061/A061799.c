/*
 * A061799  Smallest number with at least n divisors.
 * https://oeis.org/A061799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 120, u32 n <= 1920, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = minimum m such that τ(m) ≥ n
 * where τ(m) is the number‑of‑divisors function.
 *
 * Compute minimal numbers for every divisor count up to 20000 by a
 * depth‑first search over exponent vectors with non‑increasing exponents
 * (the standard construction for highly composite numbers).
 * Then take a prefix minimum so that a(n) is the smallest number with
 * at least n divisors.
 */

#include <stdint.h>
#include <limits.h>

/* ---- constants ----------------------------------------------------- */

static const int PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};
#define NUM_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

/* maximum divisor count we are prepared to answer. */
#define MAX_N 20000

/* ---- global arrays ------------------------------------------------- */

/* best_exact[d]   : minimal m with exactly d divisors */
static unsigned __int128 best_exact[MAX_N + 1];
/* best_atleast[d] : minimal m with at least d divisors */
static unsigned __int128 best_atleast[MAX_N + 2];
static int initialized = 0;

/* ---- helper functions ----------------------------------------------- */

/* recursive search that keeps exponents non‑increasing */
static void dfs(int prime_idx, int last_exp, unsigned __int128 cur_val,
                int cur_div)
{
    if (cur_div <= MAX_N && cur_val < best_exact[cur_div])
        best_exact[cur_div] = cur_val;

    if (prime_idx >= NUM_PRIMES) return;

    unsigned __int128 val = cur_val;
    for (int e = 1; e <= last_exp; ++e) {
        /* safe multiplication : stop if would overflow __int128 */
        if (val > ((unsigned __int128)(-1)) / PRIMES[prime_idx])
            break;
        val *= PRIMES[prime_idx];

        int new_div = cur_div * (e + 1);
        if (new_div > MAX_N) break;

        dfs(prime_idx + 1, e, val, new_div);
    }
}

/* initialise all data structures */
static void init(void)
{
    if (initialized) return;
    for (int i = 0; i <= MAX_N; ++i)
        best_exact[i] = (unsigned __int128)-1;
    best_exact[1] = 1;           /* τ(1) = 1 */

    dfs(0, 60, (unsigned __int128)1, 1);   /* start search */

    /* build best_atleast array */
    best_atleast[MAX_N + 1] = (unsigned __int128)-1;
    for (int d = MAX_N; d >= 1; --d) {
        unsigned __int128 v = best_exact[d];
        if (v == (unsigned __int128)-1) v = best_atleast[d + 1];
        else if (best_atleast[d + 1] < v) v = best_atleast[d + 1];
        best_atleast[d] = v;
    }
    initialized = 1;
}

/* ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of range, return 0 */
    if (n > (idx_t)MAX_N) {
        /* compute on demand: extend arrays (not needed for supplied limits) */
        /* for safety, return 0 if out of pre‑computed range */
        return (seq_t)0;
    }

    init();
    return (seq_t)best_atleast[(int)n];
}
