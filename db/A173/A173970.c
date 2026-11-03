/*
 * A173970  Primes p such that 2*p-NextPrime are prime numbers.
 * https://oeis.org/A173970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 633, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9679 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime p such that 2*p - next_prime(p+1) is prime.
 * The function lazily generates primes using the provided next_prime() helper,
 * tests the condition with is_prime_big(), and stores found values in a
 * dynamically resizable array.  Results are memoised so that subsequent
 * calls, even out of order, can be answered in O(1).
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Note: seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Memoisation data structure */
    static seq_t *results = NULL;
    static size_t results_cnt = 0;   /* number of terms stored */
    static size_t results_cap = 0;   /* allocated capacity */

    /* Prime generator state */
    static uint64_t next_to_test = 2;   /* start just before the first prime */

    /* Initialise result storage on first invocation */
    if (results == NULL) {
        results_cap = 16;
        results = seq_alloc_array(results_cap);
    }

    /* Generate terms until we have at least n of them */
    while (results_cnt < (size_t)n) {
        /* Advance to the next prime to test */
        next_to_test = next_prime(next_to_test + 1);
        uint64_t p = next_to_test;

        /* Prime immediately following p */
        uint64_t q = next_prime(p + 1);

        /* Compute r = 2*p - q; skip if not positive */
        uint64_t r_val;
        if (p + p > q) {
            r_val = p + p - q;
        } else {
            continue;   /* underflow would occur, skip this p */
        }

        /* Test whether r is prime */
        if (is_prime_big((seq_t)r_val)) {
            /* Ensure capacity for the new result */
            if (results_cnt >= results_cap) {
                size_t new_cap = results_cap * 2;
                seq_t *tmp = seq_alloc_array(new_cap);
                mem_copy(tmp, results, results_cnt * sizeof(seq_t));
                free_seq_array(results);
                results = tmp;
                results_cap = new_cap;
            }
            /* Store the prime p (converted to seq_t) */
            results[results_cnt++] = (seq_t)p;
        }
    }

    /* Return the desired term (indexing starts at 1) */
    return results[n - 1];
}
