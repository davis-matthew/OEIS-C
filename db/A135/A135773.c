/*
 * A135773  Numbers having number of divisors equal to number of digits in
 *          base 3.
 * https://oeis.org/A135773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1250, u32 n <= 1250, u64 n <= 1250,
 *            u128 n <= 1250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A135773 sequence: Numbers whose number of divisors equals
   the number of base‑3 digits.  The values are found by a search that
   tests each integer in ascending order and keeps the results in a
   static table for future calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- basic helper: integer square root ----- */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = 0;
    while (r * r <= n) r++;
    return r - 1;
}

/* ----- prime helper: dynamic list of primes ----- */
static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;

static void grow_primes(size_t needed) {
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = new_cap;
}

/* generate next prime after the current last prime */
static void push_next_prime(void) {
    uint64_t cand = (prime_len == 0) ? 2 : prime_list[prime_len - 1] + 1;
    for (;;) {
        bool is_p = true;
        uint64_t lim = isqrt_u64(cand);
        for (size_t i = 0; i < prime_len && prime_list[i] <= lim; ++i) {
            if (cand % prime_list[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            grow_primes(prime_len + 1);
            prime_list[prime_len++] = cand;
            return;
        }
        ++cand;
    }
}

/* ensure primes list covers all primes up to sqrt(n) */
static void ensure_primes_upto_uint64(uint64_t n) {
    while (prime_len == 0 || prime_list[prime_len - 1] * prime_list[prime_len - 1] <= n) {
        push_next_prime();
    }
}

/* ----- divisor count for uint64_t ----- */
static uint64_t divisor_count_u64(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t m = n;
    uint64_t tau = 1;
    ensure_primes_upto_uint64(n);
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m) break;
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (uint64_t)(exp + 1);
        }
    }
    if (m > 1) tau *= 2;       /* remaining prime factor */
    return tau;
}

/* ----- base-3 digit count ----- */
static uint64_t digits_in_base3(uint64_t n) {
    uint64_t count = 0;
    do {
        ++count;
        n /= 3;
    } while (n);
    return count;
}

/* ----- sequence storage ----- */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint64_t current_candidate = 1;   /* next integer to test */

/* grow sequence array to accommodate requested length */
static void grow_seq(size_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* ----- main function ----- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* generate sequence entries until we have at least n */
    while (seq_len < (size_t)n) {
        uint64_t x = current_candidate++;
        uint64_t tau = divisor_count_u64(x);
        uint64_t digs = digits_in_base3(x);
        if (tau == digs) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)x;
        }
    }

    return seq[n - 1];
}
