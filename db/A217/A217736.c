/*
 * A217736  Sum of first n squares of semiprimes.
 * https://oeis.org/A217736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of squares of the first n semiprimes (numbers with
 *            ω(n)=2)
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1010, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test for primality of a 64‑bit integer --------------------------------- */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (idx_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0)   return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

/* Test whether a number is a semiprime: product of two primes (possibly equal) */
static bool is_semiprime(idx_t n)
{
    if (n < 4) return false;        /* smallest semiprime is 4 = 2*2 */

    /* try to find the smallest divisor of n */
    if (n % 2 == 0) {
        idx_t q = n / 2;
        return is_prime(q);
    }
    if (n % 3 == 0) {
        idx_t q = n / 3;
        return is_prime(q);
    }
    for (idx_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0) {
            idx_t q = n / i;
            return is_prime(q);
        }
        if (n % (i + 2) == 0) {
            idx_t q = n / (i + 2);
            return is_prime(q);
        }
    }
    return false; /* n is prime */
}

/* ---------- memoised prefix sums of squares of semiprimes ---------- */
static seq_t *pref = NULL;   /* cumulative sums of semiprime squares */
static size_t pref_len = 0;   /* number of terms already computed     */
static size_t pref_cap = 0;   /* allocated capacity                   */
static idx_t  current_num = 1;   /* next integer to test for semiprime   */

/* Ensure that pref[n] (1‑based) exists – compute more semiprimes if needed */
static void ensure_pref(size_t needed)
{
    if (pref_len >= needed) return;

    /* Allocate or grow the prefix array if necessary */
    if (pref_cap == 0) {
        pref_cap = 16;
        pref = (seq_t *)malloc(pref_cap * sizeof(seq_t));
        if (!pref) return; /* allocation failure – callers will see 0 */
    }

    /* Generate semiprimes until we have enough terms */
    while (pref_len < needed) {
        ++current_num;
        if (is_semiprime(current_num)) {
            /* enlarge array if full */
            if (pref_len == pref_cap) {
                size_t new_cap = pref_cap * 2;
                seq_t *tmp = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
                if (!tmp) return;     /* allocation failure */
                pref = tmp;
                pref_cap = new_cap;
            }

            seq_t sq  = (seq_t)current_num * (seq_t)current_num;
            seq_t prev = pref_len ? pref[pref_len - 1] : (seq_t)0;
            pref[pref_len++] = prev + sq;
        }
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_pref((size_t)n);
    return pref[n - 1];
}
