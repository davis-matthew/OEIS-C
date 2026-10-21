/*
 * A092148  Expansion of e.g.f. 1/(exp(x)-x*exp(2*x)).
 * https://oeis.org/A092148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n-1} (n-k-1)^k/k! + (-1)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the explicit summation given in OEIS A092148 */
#include <stdlib.h>
#include "../utils/big_pow.h"

/* static storage for factorials */
static seq_t *fact = NULL;
static size_t fact_cap = 0;
static idx_t  fact_len = 0;

/* ensure factorial array contains values up to n */
static void
ensure_fact(idx_t n)
{
    /* no work if already computed */
    if (fact_len > n) return;
    size_t need = (size_t)n + 1;
    if (need > fact_cap) {
        seq_t *tmp = (seq_t *)realloc(fact, need * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        fact = tmp;
        fact_cap = need;
    }
    for (idx_t i = fact_len; i <= n; ++i) {
        if (i == 0) {
            fact[0] = 1;
        } else {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
    }
    fact_len = n + 1;
}

/* main entry point */
seq_t
a(idx_t n)
{
    /* Compute factorials up to n */
    ensure_fact(n);
    seq_t fact_n = fact[n];
    seq_t sum = 0;

    /* sum over k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        /* base = n-k-1 is always non‑negative here */
        seq_t base = (seq_t)(n - k - 1);
        seq_t powv = big_pow(base, (seq_t)k);
        /* term = (n! / k!) * base^k */
        seq_t term = (fact_n / fact[k]) * powv;
        sum += term;
    }

    /* add the final (-1)^n term */
    if ((n & (idx_t)1) == 0) {
        sum += 1; /* (-1)^n = 1 for even n */
    } else {
        sum -= 1; /* (-1)^n = -1 for odd n, safe because sum >= 1 */
    }

    return sum;
}
