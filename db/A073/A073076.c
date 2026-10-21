/*
 * A073076  Numbers k such that 2*k+1 divides C(2*k,k).
 * https://oeis.org/A073076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 3737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that (2*k+1) divides C(2*k,k).
   The sequence is OEIS A073076.  The function is independent of seq_t
   size and works for any idx_t (uint64_t) value of k. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binom_exponent.h"

#define MAX_PRIME 2000000               /* upper bound for pre‑computed primes */
static bool is_composite[MAX_PRIME + 1];
static idx_t primes[200000];        /* enough for primes up to MAX_PRIME */
static size_t primes_cnt = 0;

/* Generate all primes ≤ MAX_PRIME once. */
static void generate_primes(void)
{
    static bool initialized = false;
    if (initialized) return;
    for (idx_t i = 2; i <= MAX_PRIME; ++i) is_composite[i] = false;
    for (idx_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = i;
            if (i * i <= MAX_PRIME) {
                for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    initialized = true;
}

/* Test whether k satisfies 2k+1 | C(2k,k). */
static bool ok_k(idx_t k)
{
    idx_t m = 2 * k + 1;
    idx_t temp = m;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }
            if (binom_exponent(k, p) < exp) return false;
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        if (binom_exponent(k, temp) < 1) return false;
    }
    return true;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    generate_primes();

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (ok_k(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
