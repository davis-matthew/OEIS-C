/*
 * A063244  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          94 ).
 * https://oeis.org/A063244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(1)=3, a(2)=12, a(3)=20,
 *            a(4)=26, a(5)=34, a(6)=44, a(n)=a(n‑2)+a(n‑3)−a(n‑5) for n ≥ 7
 * Range:     u8 n <= 33, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoised values (1-indexed, index 0 unused) */
static seq_t *memo = NULL;      /* array pointer */
static size_t memo_cap = 0;     /* current capacity (>= len) */
static size_t memo_len = 0;     /* number of filled positions (>= memo_len) */

/* ensure capacity to hold at least “new_cap” elements */
static void ensure_cap(size_t new_cap) {
    if (new_cap <= memo_cap)
        return;
    size_t new_size = new_cap;
    /* simple growth strategy: double until big enough */
    while (new_size < new_cap)
        new_size = new_size * 2 + 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    memo = new_mem;
    memo_cap = new_size;
}

/* initialise the memoisation table with the first six terms */
static void init_memo(void) {
    if (memo != NULL)
        return;                /* already initialised */
    ensure_cap(7);              /* indices 0..6 */
    memo[0] = (seq_t)0;         /* unused */
    memo[1] = (seq_t)3;
    memo[2] = (seq_t)12;
    memo[3] = (seq_t)20;
    memo[4] = (seq_t)26;
    memo[5] = (seq_t)34;
    memo[6] = (seq_t)44;
    memo_len = 7;
}

/* compute terms up to n (1‑based) using the recurrence */
static void compute_up_to(idx_t n) {
    /* n is 1‑based, convert to size_t; assume it fits */
    size_t idx = (size_t)n;
    if (idx < memo_len)
        return;
    ensure_cap(idx + 1);
    for (size_t i = memo_len; i <= idx; ++i) {
        /* a(i) = a(i-2) + a(i-3) - a(i-5) */
        seq_t sum = memo[i - 2] + memo[i - 3];
        memo[i] = sum - memo[i - 5];
    }
    memo_len = idx + 1;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;      /* undefined for n=0 – return 0 */
    init_memo();
    compute_up_to(n);
    return memo[(size_t)n];
}
