/*
 * A230423  a(n) = smallest natural number x such that x=n+A034968(x), or zero
 *          if no such number exists.
 * https://oeis.org/A230423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A230414(n)
 * Range:     u8 n <= 248, u16 n <= 5714, u32 n <= 9466, u64 n <= 9466,
 *            u128 n <= 9466, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

enum { FACT_MAX = 12 };          /* enough for n up to at least 10⁴ */
static seq_t fact[FACT_MAX + 1];
static bool fact_inited = false;

static void init_fact(void)
{
    if (fact_inited) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= FACT_MAX; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_inited = true;
}

seq_t a(idx_t n)
{
    init_fact();

    seq_t sn = (seq_t)n;                /* work in seq_t to avoid overflows */

    /* Find the largest k with (k! – 1) ≤ sn. */
    int max_k = 0;
    for (int k = FACT_MAX; k >= 2; --k) {
        seq_t weight = fact[k] - (seq_t)1;
        if (weight <= sn) {
            max_k = k;
            break;
        }
    }
    if (max_k < 2) return (seq_t)0;     /* no representation possible */

    seq_t rem    = sn;
    seq_t result = (seq_t)0;

    /* Greedy decomposition using the factorial‑based weights. */
    for (int k = max_k; k >= 2; --k) {
        seq_t weight = fact[k] - (seq_t)1;
        if (weight == (seq_t)0 || weight > rem) continue;

        seq_t d    = rem / weight;
        seq_t kseq = (seq_t)k;
        if (d > kseq) d = kseq;          /* digit bound */

        rem    -= d * weight;
        result += d * fact[k];
    }

    return (rem == (seq_t)0) ? result : (seq_t)0;
}
