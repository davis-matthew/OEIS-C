/*
 * A074190  Let the index of the largest prime power that divides n! be k then
 *          the smallest number such that n!*a(n) is a perfect k-th power.
 * https://oeis.org/A074190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n} p^{k - v_p(n!)} where k = max_{p≤n} v_p(n!)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

#define MAX_N 10000

/* Cached list of primes up to MAX_N */
static bool primes_computed = false;
static int primes[1229];
static size_t prime_cnt = 0;

static void compute_primes(void)
{
    if (primes_computed) return;
    bool is_composite[MAX_N + 1] = {0};
    prime_cnt = 0;
    for (int i = 2; i <= MAX_N; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= MAX_N) {
                for (int j = i * i; j <= MAX_N; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_computed = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;      /* safety for n < 1, though calls start at 1 */

    compute_primes();

    /* Exponents of each prime in n! and the maximum exponent k */
    static idx_t exps[1229];
    idx_t k = 0;
    size_t i;
    for (i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        int p = primes[i];
        idx_t e = 0;
        idx_t div = n;
        while (div) {
            div /= p;
            e += div;
        }
        exps[i] = e;
        if (e > k) k = e;
    }

    /* Build the result ∏ p^{k - e_p} */
    seq_t result = 1;
    for (size_t j = 0; j < i; ++j) {
        idx_t diff = k - exps[j];
        if (diff > 0) {
            seq_t pow_val = big_pow((seq_t)primes[j], (seq_t)diff);
            result *= pow_val;
        }
    }
    return result;
}
