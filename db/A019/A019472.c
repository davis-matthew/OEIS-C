/*
 * A019472  Weak preference orderings of n alternatives, i.e., orderings that
 *          have indifference between at least two alternatives.
 * https://oeis.org/A019472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000670(n) - n! (weak preference orderings)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Initial capacity for dynamic arrays. It can grow if needed. */
#define INITIAL_CAP 16

/* Static state for memoisation. */
static seq_t *dp = NULL;        /* A000670 values (preferential arrangements) */
static seq_t *fact = NULL;      /* factorials  */
static size_t cap = 0;          /* current allocation size (number of elements) */
static idx_t  computed = 0;     /* largest n that has been computed */
static bool   initialized = false;

/* Allocate and initialise the arrays. */
static void init(void) {
    if (initialized) return;
    cap = INITIAL_CAP;
    if (cap < 1) cap = 1; /* at least room for dp[0] and fact[0] */
    dp = (seq_t *)malloc(cap * sizeof(seq_t));
    fact = (seq_t *)malloc(cap * sizeof(seq_t));
    dp[0] = (seq_t)1;
    fact[0] = (seq_t)1;
    computed = 0;
    initialized = true;
}

/* Expand the arrays to hold at least n+1 elements. */
static void ensure_cap(idx_t n) {
    if ((size_t)(n + 1) <= cap) return;   /* already sufficient */
    size_t new_cap = cap ? cap : 1;
    while ((size_t)(n + 1) > new_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    dp = tmp;
    tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    fact = tmp;
    cap = new_cap;
}

/* Compute dp[i] (A000670) and fact[i] for all i up to n (inclusive). */
static void compute_up_to(idx_t n) {
    init();
    if (n <= computed) return;
    ensure_cap(n);
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* compute dp[i] = sum_{k=0}^{i-1} C(i,k) * dp[k]  */
        seq_t sum = 0;
        idx_t C = 1;                   /* C(i,0) = 1 */
        for (idx_t k = 0; k < i; ++k) {
            sum += dp[k] * (seq_t)C;
            C = C * (i - k) / (k + 1); /* next binomial coefficient */
        }
        dp[i] = sum;
        /* compute i!  */
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    computed = n;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    compute_up_to(n);
    return dp[n] - fact[n];
}
