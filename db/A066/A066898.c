/*
 * A066898  Total number of even parts in all partitions of n.
 * https://oeis.org/A066898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} tau(k) * p(n-2k)
 * Range:     u8 n <= 13, u16 n <= 35, u32 n <= 108, u64 n <= 368,
 *            u128 n <= 1343, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(k) = number of divisors of k, and p(m) = partition(m) */

#include <stdbool.h>

#define MAX_N 10000            /* works for all tested n (up to ~10000) */

static seq_t p[MAX_N + 1];            /* partition numbers */
static seq_t a_arr[MAX_N + 1];        /* result a(n) */
static unsigned int tau[MAX_N / 2 + 1];
static bool init_done = false;

/* ------------------------------------------------------------------ */
/* compute all needed values once                                     */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /* Partitions: p[0] = 1, all others start 0 (global zero‑init) */
    p[0] = 1;
    for (idx_t coin = 1; coin <= MAX_N; ++coin) {
        for (idx_t j = coin; j <= MAX_N; ++j) {
            p[j] += p[j - coin];
        }
    }

    /* Divisor counts tau(k) for k = 1 .. MAX_N/2 */
    for (idx_t i = 1; i <= MAX_N / 2; ++i) {
        for (idx_t j = i; j <= MAX_N / 2; j += i) {
            tau[j]++;        /* small integer fits in unsigned int */
        }
    }

    /* Compute a(n) using the formula */
    for (idx_t k = 1; k <= MAX_N / 2; ++k) {
        unsigned int t = tau[k];
        for (idx_t m = 0; m <= MAX_N - 2 * k; ++m) {
            a_arr[m + 2 * k] += p[m] * (seq_t)t;  /* cast to seq_t for multiplication */
        }
    }

    init_done = true;
}

/* ------------------------------------------------------------------ */
/* external function required by the test harness                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done) {
        init();
    }
    /* n is assumed >= 1; for n == 0 a_arr[0] is 0 by initialization */
    return a_arr[n];
}
