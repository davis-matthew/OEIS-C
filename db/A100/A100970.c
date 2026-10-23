/*
 * A100970  Integers that are Rhonda numbers to base 8.
 * https://oeis.org/A100970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = nth Rhonda number to base 8 Rhonda number: product of
 *            base‑8 digits = 8 × sum of prime factors (with multiplicity)
 * Range:     u8 none, u16 n <= 21, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum prime needed for trial division – enough for candidates up
   to about 10^10, more than sufficient for n ≤ 10000. */
#define MAX_PRIME 100000

static int primes[MAX_PRIME + 1];
static int prime_count = 0;
static bool primes_ready = false;

/* Simple sieve – called once on first use. */
static void init_primes(void)
{
    if (primes_ready) return;
    bool *sieve = (bool *)malloc((MAX_PRIME + 1) * sizeof(bool));
    if (!sieve) abort();
    for (int i = 0; i <= MAX_PRIME; ++i) sieve[i] = false;

    for (int p = 2; p <= MAX_PRIME; ++p) {
        if (!sieve[p]) {
            primes[prime_count++] = p;
            if ((int64_t)p * p <= MAX_PRIME) {
                for (int64_t m = (int64_t)p * p; m <= MAX_PRIME; m += p)
                    sieve[(int)m] = true;
            }
        }
    }
    free(sieve);
    primes_ready = true;
}

/* Sum of prime factors of x, counting multiplicity:
   sum = Σ (e_i · p_i) for each prime p_i^e_i || x. */
static seq_t prime_factor_sum(seq_t x)
{
    if (x <= 1) return 0;
    idx_t m = (idx_t)x;
    seq_t sum = 0;
    for (int i = 0; i < prime_count; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > m) break;
        while (m % p == 0) {
            sum += (seq_t)p;
            m /= p;
        }
    }
    if (m > 1) sum += (seq_t)m;   /* remaining prime factor */
    return sum;
}

/* Store up to 20000 terms statically to avoid dynamic allocation. */
#define MAX_TERMS 20000
static seq_t terms[MAX_TERMS];
static idx_t terms_cnt = 0;
static seq_t next_candidate = 1;

/* Append new term to the table. */
static void add_term(seq_t value)
{
    if (terms_cnt >= MAX_TERMS) abort(); /* overflow safeguard */
    terms[terms_cnt++] = value;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, harmless */

    init_primes();

    while (terms_cnt < n) {
        seq_t cand = next_candidate++;
        /* Compute product of base‑8 digits, skipping numbers with a 0 digit. */
        seq_t prod = 1;
        bool zero_digit = false;
        seq_t tmp = cand;
        while (tmp > 0) {
            int digit = (int)(tmp % 8);     /* remainder 0‑7 */
            if (digit == 0) { zero_digit = true; break; }
            prod *= (seq_t)digit;
            tmp /= 8;
        }
        if (zero_digit) continue;
        if ((prod % 8) != 0) continue;     /* must be divisible by 8 */

        seq_t target = prod / 8;            /* required sum of prime factors */
        seq_t sum    = prime_factor_sum(cand);
        if (sum == target) add_term(cand);
    }
    return terms[n - 1];
}
