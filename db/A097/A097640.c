/*
 * A097640  Numbers n such that n=(d_1+d_2+...+d_k)*prime(d_1+d_2+...+d_k)
 *          where d_1 d_2 ... d_k is the decimal expansion of n.
 * https://oeis.org/A097640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   n is in the sequence iff n = s * prime(s) and digit_sum(n) = s
 *            where prime(k) is the k-th prime (prime(1)=2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper: digit sum of a seq_t value ---------- */
static seq_t digit_sum_seq_t(seq_t v) {
    seq_t sum = 0;
    while (v) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* ---------- Prime generator: sieve up to a fixed limit ---------- */
#define SIEVE_LIMIT 8000000          /* enough for ~500 000 primes */
static bool primes_inited = false;
static int *primes = NULL;          /* 1‑based index: primes[0] = 2 */
static size_t prime_cnt = 0;

/* initialize the prime table lazily */
static void init_primes(void) {
    if (primes_inited) return;
    primes_inited = true;

    bool *is_prime = (bool *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) exit(1);
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;

    size_t cap = 530000;            /* adequate to store ~500k primes */
    primes = (int *)malloc(cap * sizeof(int));
    if (!primes) exit(1);
    prime_cnt = 0;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_cnt >= cap) {
                cap *= 2;
                primes = (int *)realloc(primes, cap * sizeof(int));
                if (!primes) exit(1);
            }
            primes[prime_cnt++] = (int)i;
            if (i * i <= SIEVE_LIMIT)
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) is_prime[j] = false;
        }
    }
    free(is_prime);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static state: cached terms, search cursor, etc. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_cnt = 0;
    static idx_t next_s = 1;       /* next digit sum to test */

    /* Ensure primes are ready */
    init_primes();

    /* Grow terms array if needed */
    if (terms_cnt < (size_t)n) {
        if (!terms) {
            terms_cap = 1024;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            if (!terms) exit(1);
        }
        while (terms_cnt < (size_t)n) {
            /* make sure we have a prime for this s */
            if ((size_t)next_s > prime_cnt) {
                /* shouldn't happen with current sieve limit */
                exit(1);
            }
            seq_t p = (seq_t)primes[(size_t)next_s - 1];
            seq_t val = (seq_t)next_s * p;
            if (digit_sum_seq_t(val) == (seq_t)next_s) {
                /* store term */
                if (terms_cnt >= terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!terms) exit(1);
                }
                terms[terms_cnt++] = val;
            }
            ++next_s;
        }
    }

    return terms[(size_t)n - 1];
}
