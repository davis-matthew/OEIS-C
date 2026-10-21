/*
 * A095082  Fib00 primes, i.e., primes p whose Zeckendorf-expansion A014417(p)
 *          ends with two zeros.
 * https://oeis.org/A095082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th prime whose Zeckendorf expansion ends with two zeros.
   The function searches successive odd candidates, tests primality by trial
   division using previously found primes, and checks the Zeckendorf digits
   by greedy subtraction of Fibonacci numbers.  The result is memoised
   in static arrays so repeated calls in any order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Fibonacci numbers, fib_arr[0] = 1, fib_arr[1] = 2 */
static seq_t *fib_arr = NULL;
static size_t fib_len = 0;

/* All primes found so far – used for trial‑division */
static uint64_t *prime_arr = NULL;
static size_t prime_cap = 0, prime_len = 0;

/* Primes satisfying the Zeckendorf condition */
static seq_t *fib00_arr = NULL;
static size_t fib00_cap = 0, fib00_len = 0;

/* Next odd candidate to test for primality */
static uint64_t next_candidate = 3;

/* Initialise the Fibonacci table with 1,2. */
static void init_fib(void)
{
    fib_len = 2;
    fib_arr = (seq_t *)realloc(fib_arr, fib_len * sizeof(seq_t));
    fib_arr[0] = (seq_t)1;
    fib_arr[1] = (seq_t)2;
}

/* Ensure fib_arr contains all Fibonacci numbers <= value. */
static void ensure_fib_upto(seq_t value)
{
    if (fib_len == 0)
        init_fib();
    while (fib_arr[fib_len - 1] <= value) {
        seq_t next = fib_arr[fib_len - 1] + fib_arr[fib_len - 2];
        fib_arr = (seq_t *)realloc(fib_arr, (fib_len + 1) * sizeof(seq_t));
        fib_arr[fib_len++] = next;
    }
}

/* Return true iff the Zeckendorf representation of n ends with two zeros,
   i.e. the representation does not use 1 or 2. */
static bool has_two_trailing_zeros(seq_t n)
{
    seq_t rem = n;
    while (rem > (seq_t)0) {
        /* find largest fib <= rem */
        size_t i = fib_len - 1;
        while (i > 0 && fib_arr[i] > rem)
            --i;
        seq_t fib_val = fib_arr[i];
        if (fib_val == (seq_t)1 || fib_val == (seq_t)2)
            return false;           /* 1 or 2 would appear */
        rem -= fib_val;
    }
    return true;
}

/* Append a prime to prime_arr, expanding the buffer as needed. */
static void add_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    }
    prime_arr[prime_len++] = p;
}

/* Append a qualifying prime (fits in seq_t) to fib00_arr. */
static void add_fib00(seq_t p)
{
    if (fib00_len == fib00_cap) {
        fib00_cap = fib00_cap ? fib00_cap * 2 : 16;
        fib00_arr = (seq_t *)realloc(fib00_arr, fib00_cap * sizeof(seq_t));
    }
    fib00_arr[fib00_len++] = p;
}

/* Main routine: return the n-th prime in A095082. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (fib00_len < n) {
        uint64_t cand;
        if (prime_len == 0) {                  /* initialisation */
            add_prime(2);                      /* 2 is not a Fib00 prime */
            cand = 3;
            next_candidate = 5;                /* next odd after 3 */
        } else {
            cand = next_candidate;
            next_candidate += 2;               /* skip even numbers */
        }

        /* Primality test using primes we have collected. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_arr[i];
            if (p > cand / p) break;          /* p*p > cand */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        add_prime(cand);                       /* cache for future tests */

        /* Ensure Fibonacci list is up to date. */
        ensure_fib_upto((seq_t)cand);

        /* Check the Zeckendorf condition. */
        if (has_two_trailing_zeros((seq_t)cand))
            add_fib00((seq_t)cand);
    }

    return fib00_arr[n - 1];
}
