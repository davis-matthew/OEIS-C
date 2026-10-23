/*
 * A024556  Odd squarefree composite numbers.
 * https://oeis.org/A024556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 9379, u32 n <= 9379, u64 n <= 9379,
 *            u128 n <= 9379, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd squarefree composite numbers.
   Computation: iterate odd numbers, test for composite and squarefree, and
   return the nth such number. This satisfies the OEIS definition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

/* --- prime list state -------------------------------------------------- */
static uint32_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static size_t   prime_alloc   = 0;
static idx_t    prime_limit   = 1;

/* Ensure prime_list contains all primes up to 'limit'. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* enlarge allocation if necessary */
    if (limit > prime_alloc) {
        size_t new_alloc = prime_alloc ? prime_alloc : 16;
        while (new_alloc <= limit)
            new_alloc <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_alloc * sizeof(uint32_t));
        if (!tmp) exit(1);                  /* memory failure */
        prime_list = tmp;
        prime_alloc = new_alloc;
    }

    /* trial‑division sieve from current limit+1 up to 'limit' */
    for (idx_t cand = prime_limit + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        idx_t root = int_sqrt(cand);
        for (size_t i = 0; i < prime_count && prime_list[i] <= root; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_alloc) {            /* grow if full */
                size_t new_alloc = prime_alloc << 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, new_alloc * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_alloc = new_alloc;
            }
            prime_list[prime_count++] = (uint32_t)cand;
        }
    }
    prime_limit = limit;
}

/* Test whether 'value' is odd, composite, and squarefree. */
static bool is_squarefree_composite(idx_t value)
{
    if (value < 9)         /* the smallest odd squarefree composite is 15 */
        return false;
    if ((value & 1ULL) == 0)   /* even numbers excluded */
        return false;
    if (value <= 3)
        return false;      /* 3 is prime */

    idx_t tmp = value;
    bool squarefree = true;
    size_t distinct = 0;

    idx_t limit = int_sqrt(tmp);
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_count && (idx_t)prime_list[i] <= limit; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            ++distinct;
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                if (exp > 1) {          /* repeated prime factor */
                    squarefree = false;
                    break;
                }
            }
            if (!squarefree)
                return false;
            limit = int_sqrt(tmp);      /* remaining part may be smaller */
        }
    }

    if (tmp > 1)                 /* remaining prime factor > sqrt(original) */
        ++distinct;

    return squarefree && distinct >= 2;   /* composite requires at least 2 distinct primes */
}

/* Return the nth odd squarefree composite number (1‑based indexing). */
seq_t a(idx_t n)
{
    /* initialise the prime list once */
    static bool initialized = false;
    if (!initialized) {
        prime_alloc = 16;
        prime_list  = (uint32_t *)calloc(prime_alloc, sizeof(uint32_t));
        if (!prime_list) exit(1);
        prime_list[0] = 2;          /* prime 2, used for trial division */
        prime_count = 1;
        prime_limit = 2;
        initialized = true;
    }

    idx_t count = 0;
    idx_t candidate = 3;               /* start from the smallest odd number >2 */

    while (true) {
        if (is_squarefree_composite(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;                  /* skip even numbers */
    }
}
