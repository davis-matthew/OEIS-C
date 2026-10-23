/*
 * A155478  Even composites n such that smallest digit of n > smallest digit
 *          of n-th prime.
 * https://oeis.org/A155478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smallest_digit_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Extension: a(n) = the nth even composite n such that the smallest
   decimal digit of n is greater than the smallest digit of the n‑th
   prime. This is computed by testing successive even composites and
   caching the results. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/smallest_digit_seq.h"

/* Prime storage and generation */
static seq_t *primes = NULL;
static size_t primes_cap = 0;
static idx_t prime_cnt = 0;
static uint64_t prime_next_check = 5;

static void init_primes(void)
{
    if (!primes) {
        primes_cap = 8;
        primes = (seq_t *)malloc(sizeof(seq_t) * primes_cap);
        primes[0] = 2;
        primes[1] = 3;
        prime_cnt = 2;
        prime_next_check = 5;
    }
}

static void ensure_primes_at_least(idx_t need)
{
    init_primes();
    while (prime_cnt < need) {
        seq_t p = prime_next_check;
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            seq_t pr = primes[i];
            if ((unsigned __int128)pr * pr > p) break;
            if (p % pr == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == primes_cap) {
                primes_cap = primes_cap * 2;
                primes = (seq_t *)realloc(primes, sizeof(seq_t) * primes_cap);
            }
            primes[prime_cnt++] = p;
        }
        prime_next_check += 2; /* skip even numbers */
    }
}

/* Sequence cache */
static seq_t *answers = NULL;
static size_t ans_cap = 0;
static size_t ans_cnt = 0;
static uint64_t next_candidate = 4; /* first even composite */

static void ensure_answer(idx_t target)
{
    /* target is 1‑based index of sequence term. */
    while (ans_cnt < target) {
        ensure_primes_at_least(next_candidate);
        seq_t prime_val = primes[next_candidate - 1];
        int min_digit_prime = smallest_digit_seq(prime_val);
        int min_digit_candidate = smallest_digit_seq(next_candidate);
        if (min_digit_candidate > min_digit_prime) {
            if (ans_cnt == ans_cap) {
                ans_cap = ans_cap ? ans_cap * 2 : 8;
                answers = (seq_t *)realloc(answers, sizeof(seq_t) * ans_cap);
            }
            answers[ans_cnt++] = (seq_t)next_candidate;
        }
        next_candidate += 2; /* next even number */
    }
}

/* Main entry function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, defensive */
    ensure_answer(n);
    return answers[n - 1];
}
