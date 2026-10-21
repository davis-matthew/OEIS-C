/*
 * A051046  Numbers k for which pi(k) > k/(H_k - 3/2), where pi is the
 *          prime-counting function and H_k is the k-th harmonic number.
 * https://oeis.org/A051046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A051046:
   Numbers k for which π(k) > k/(H_k – 3/2).
   The sequence is known to contain every integer k ≥ 1429 (Panaitopol 1999),
   so after precomputing all qualifying k ≤ 1428 we can answer any n in O(1). */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *pre = NULL;             /* array of early terms (k ≤ 1428)   */
static size_t pre_len = 0;             /* number of terms stored           */
static bool initialized = false;       /* lazy initialization flag        */

/* Helper to build the first part of the sequence. */
static void init_pre(void)
{
    const int MAX_K = 1428;            /* last k that needs explicit check */
    bool is_prime[MAX_K + 1];
    int pi_cnt = 0;
    long double H = 0.0L;              /* harmonic number H_k  */
    size_t alloc = MAX_K + 1;          /* worst‑case size                     */
    pre = (seq_t *)malloc(alloc * sizeof(seq_t));
    pre_len = 0;

    /* trivial sieve up to MAX_K */
    for (int i = 0; i <= MAX_K; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_K; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_K; j += i)
                is_prime[j] = false;

    /* compute the set of k that satisfy the inequality */
    for (int k = 1; k <= MAX_K; ++k) {
        H += 1.0L / (long double)k;
        if (is_prime[k]) ++pi_cnt;
        long double thresh = (long double)k / (H - 1.5L);
        if ((long double)pi_cnt > thresh + 1e-12L) {
            pre[pre_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    /* lazy initialization of the prefix up to 1428 */
    if (!initialized) {
        init_pre();
        initialized = true;
    }

    if (n <= pre_len)               /* within precomputed range       */
        return pre[n - 1];

    /* beyond 1428 all numbers belong to the sequence */
    return (seq_t)(1429 + (n - pre_len - 1));
}
