/*
 * A051266  Numbers n such that maximal value of prime divisors of reduced
 *          residue system for n is 2.
 * https://oeis.org/A051266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 161, u32 n <= 161, u64 n <= 161,
 *            u128 n <= 161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051266
   a(n) = the n‑th integer N such that among the reduced residue system modulo N
          the largest number of distinct prime divisors of any element equals 2.
   Algorithm:
      * Pre‑compute a list of primes up to a fixed upper bound.
      * Generate the sequence incrementally: for each candidate N start from 2,
        factor N, build the set of primes ≤ N−1 that do NOT divide N.
        Let p1 < p2 < p3 be the first three such primes.
          • N is accepted if   p1·p2 < N        (there is a coprime number with 2 primes)
          • and (|S| < 3  OR  p1·p2·p3 ≥ N)   (no coprime number has 3 primes)
        Each accepted N is appended to the global array holding the sequence.
      * The function a(idx_t n) returns the n‑th term, generating more terms
        only when necessary.  All computations use unsigned arithmetic
        compatible with 64‑bit or larger seq_t types.  */

/* ----------  header includes  ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  global data  ---------- */
static uint32_t *PRIMES     = NULL;   /* array of primes */
static size_t   PRIME_CNT   = 0;      /* number of primes stored */

static seq_t   *SEQ_TERMS  = NULL;   /* generated sequence values */
static size_t   SEQ_CNT    = 0;      /* number of terms stored */

/* last integer that has been examined when generating the sequence */
static size_t  LAST_CHECKED = 0;

/* ----------  prime sieve (one‑time)  ---------- */
#define PRIME_LIMIT 2000000   /* primes up to this value are pre‑computed */

static void init_primes(void)
{
    if (PRIMES != NULL) return;   /* already initialised */

    /* simple Eratosthenes sieve, stored as a byte array */
    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes, allocate array, copy them */
    size_t count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++count;
    PRIMES = (uint32_t *)malloc(count * sizeof(uint32_t));
    PRIME_CNT = count;
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) PRIMES[idx++] = (uint32_t)i;

    free(is_prime);
}

/* ----------  helper: append a new term  ---------- */
static void append_seq(seq_t value)
{
    if (SEQ_CNT == 0) {
        SEQ_TERMS = (seq_t *)malloc(1024 * sizeof(seq_t));
    } else {
        SEQ_TERMS = (seq_t *)realloc(SEQ_TERMS, (SEQ_CNT + 1) * sizeof(seq_t));
    }
    SEQ_TERMS[SEQ_CNT++] = value;
}

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    init_primes();

    /* generate terms until we have at least n of them */
    while (SEQ_CNT < n) {
        ++LAST_CHECKED;
        uint32_t x = (uint32_t)LAST_CHECKED;   /* x fits in 32 bits for our bounds */

        /* ---- factorisation of x ---- */
        uint32_t factors[10];
        int faccnt = 0;
        uint32_t tmp = x;
        for (size_t i = 0; i < PRIME_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= tmp; ++i) {
            uint32_t p = PRIMES[i];
            if (tmp % p == 0) {
                factors[faccnt++] = p;
                while (tmp % p == 0) tmp /= p;
            }
        }
        if (tmp > 1 && faccnt < 10) {
            factors[faccnt++] = tmp;
        }

        /* ---- collect the first three primes ≤ x−1 that do NOT divide x ---- */
        uint32_t allowed[3];
        int allowed_cnt = 0;
        for (size_t i = 0; i < PRIME_CNT && PRIMES[i] < x; ++i) {
            uint32_t p = PRIMES[i];

            /* test whether p divides x */
            bool divides = false;
            for (int j = 0; j < faccnt; ++j) {
                if (factors[j] == p) { divides = true; break; }
            }
            if (!divides) {
                if (allowed_cnt < 3)
                    allowed[allowed_cnt++] = p;
                /* if we already have three, we don't need any further primes */
                if (allowed_cnt == 3) break;
            }
        }

        /* ---- check the conditions ---- */
        bool good = false;
        if (allowed_cnt >= 2) {
            uint64_t prod2 = (uint64_t)allowed[0] * allowed[1];
            if (prod2 < x) {
                if (allowed_cnt < 3) {
                    good = true;
                } else {
                    uint64_t prod3 = prod2 * (uint64_t)allowed[2];
                    if (prod3 >= x) good = true;
                }
            }
        }

        if (good) append_seq((seq_t)x);
    }

    return SEQ_TERMS[n - 1];
}
