/*
 * A100090  Smallest m greater than the n-th prime such that the arithmetic
 *          mean of m and the first n primes is an integer.
 * https://oeis.org/A100090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > p_n such that (m + sum_{i=1..n} p_i) mod
 *            (n+1) = 0
 * Range:     u8 n <= 13, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for primes and their prefix sums. */
static seq_t *prime_vals   = NULL;
static seq_t *prime_sums   = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;
static uint64_t last_checked = 1;

/* Resize the prime/value arrays to accommodate at least `min_cap` elements. */
static void prime_resize(size_t min_cap) {
    if (prime_cap >= min_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_vals = (seq_t *)realloc(prime_vals, new_cap * sizeof(seq_t));
    seq_t *new_sums = (seq_t *)realloc(prime_sums, new_cap * sizeof(seq_t));
    if (!new_vals || !new_sums) abort();
    prime_vals = new_vals;
    prime_sums = new_sums;
    prime_cap = new_cap;
}

/* Generate primes (and prefix sums) until at least `need` primes are available. */
static void ensure_primes(idx_t need) {
    while (prime_count < need) {
        ++last_checked;
        /* Test whether last_checked is composite using stored primes. */
        int composite = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = (uint64_t)prime_vals[i];
            if (p * p > last_checked) break;
            if ((seq_t)last_checked % prime_vals[i] == 0) {
                composite = 1;
                break;
            }
        }
        if (!composite) {
            seq_t p = (seq_t)last_checked;
            prime_resize(prime_count + 1);
            prime_vals[prime_count] = p;
            if (prime_count == 0)
                prime_sums[prime_count] = p;
            else
                prime_sums[prime_count] = prime_sums[prime_count - 1] + p;
            ++prime_count;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n<1, return 0 as placeholder */
    ensure_primes(n);                /* guarantee at least n primes are stored */

    seq_t nth_prime   = prime_vals[n - 1];
    seq_t sum_first_n = prime_sums[n - 1];
    seq_t mod_base    = (seq_t)(n + 1);

    /* Compute target remainder: m ≡ -(sum_first_n) (mod n+1). */
    seq_t target = ((mod_base - (sum_first_n % mod_base)) % mod_base);

    seq_t m = target;
    if (m <= nth_prime) {
        seq_t diff = nth_prime - m;
        seq_t add  = diff / mod_base + 1;
        m += add * mod_base;
    }
    return m;
}
