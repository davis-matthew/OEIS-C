/*
 * A071179  n - (sum of prime factors of n) is prime.
 * https://oeis.org/A071179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer n such that n - sum of distinct primes
 *            dividing n is prime
 * Range:     u8 n <= 80, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap = 0;

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t prime_gen_limit = 1;   /* largest number up to which primes are generated */

static uint64_t candidate = 1;      /* last tested integer */

/*** Helper: ensure prime list contains all primes ≤ u */
/*** This routine generates primes incrementally, using trial division */
/*** Only increments the list once per new prime. */
static void ensure_prime_limit_upto(uint64_t limit)
{
    if (limit <= prime_gen_limit) return;

    /* initialise prime list if empty */
    if (prime_cnt == 0) {
        prime_cap = 4;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_list[prime_cnt++] = 2;
        prime_gen_limit = 2;
    }

    uint64_t num = prime_gen_limit + 1;
    if (num <= 2) num = 3;          /* skip even numbers after 2 */

    for (; num <= limit; num += 2) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            /* stop if p² > num */
            if ((unsigned __int128)p * p > num) break;
            if (num % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 4;
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = num;
        }
    }
    prime_gen_limit = limit;
}

/*** Primality test for uint64_t using the prime list */
static bool isPrimeUint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    ensure_prime_limit_upto(n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*** Sum of distinct prime factors of x (x fits in uint64_t) */
static uint64_t sum_distinct_pf(uint64_t x)
{
    uint64_t sum = 0;
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_cnt && (unsigned __int128)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            sum += p;
            do { tmp /= p; } while (tmp % p == 0);
        }
    }
    if (tmp > 1) sum += tmp;          /* remaining prime factor */
    return sum;
}

/*** Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* not used, but safe */

    /* initialise storage for sequence values if first call */
    if (terms == NULL) {
        terms_cap = 4;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms_size = 0;
    }

    /* generate terms until we have at least n entries */
    while (terms_size < (size_t)n) {
        ++candidate;                          /* next integer to test */

        /* make sure we have primes to factor candidate */
        ensure_prime_limit_upto(candidate);

        uint64_t s = sum_distinct_pf(candidate);
        uint64_t diff = candidate - s;
        if (diff > 1 && isPrimeUint64(diff)) {
            if (terms_size == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_size++] = (seq_t)candidate;
        }
    }

    return terms[n - 1];
}
