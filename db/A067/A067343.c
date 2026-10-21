/*
 * A067343  Sum of decimal digits of n equals sum of decimal digits of sum of
 *          divisors of n.
 * https://oeis.org/A067343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th integer such that digit_sum(n) ==
 *            digit_sum(sigma(n))
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists the integers n for which the sum of decimal digits of n
   equals the sum of decimal digits of the sum of all divisors of n.
   a(1) = 1, a(2) = 15, a(3) = 24, ... */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Utility: sum of decimal digits of a seq_t number                   */
/* ------------------------------------------------------------------ */
static seq_t digit_sum(seq_t v) {
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Prime sieve: generate primes up to a fixed limit.                   */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 2000000
static unsigned char is_composite[PRIME_LIMIT + 1];
static int primes[PRIME_LIMIT / 10];
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (prime_cnt != 0) return;          /* already initialized */
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_composite[i]) continue;
        primes[prime_cnt++] = i;
        if ((long long)i * i <= PRIME_LIMIT) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Sum of divisors sigma(n) for a seq_t number.                       */
/* Uses the prime list generated above.                               */
/* ------------------------------------------------------------------ */
static seq_t sigma(seq_t n) {
    seq_t result = (seq_t)1;
    seq_t temp = n;
    init_primes();

    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        seq_t pp = (seq_t)p;
        if (pp * pp > temp) break;
        if (temp % pp == 0) {
            seq_t term = (seq_t)1;
            seq_t power = (seq_t)1;
            while (temp % pp == 0) {
                temp /= pp;
                power = power * pp;
                term = term + power;
            }
            result = result * term;
        }
    }
    if (temp > (seq_t)1) {               /* remaining prime factor */
        result = result * ((seq_t)1 + temp);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Dynamic array to cache sequence terms.                             */
/* ------------------------------------------------------------------ */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void push_value(seq_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Main function: returns the n-th term of the sequence.              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, fallback */

    static uint64_t next_candidate = 1;

    while (seq_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t sig = sigma(cand);
        if (digit_sum(cand) == digit_sum(sig)) {
            push_value(cand);
        }
        ++next_candidate;
    }
    return seq_vals[n - 1];
}
