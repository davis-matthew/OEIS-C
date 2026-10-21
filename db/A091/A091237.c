/*
 * A091237  Lengths of runs in A039702.
 * https://oeis.org/A091237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  OEIS A091237 – Lengths of runs in A039702 (primes modulo 4)
 *
 *  The sequence consists of the lengths of consecutive runs of primes
 *  that share the same residue modulo 4.  For example, the primes
 *  2,3,5,7,11,13,17,19,23,… give the runs
 *          [2]          length 1
 *          [3]          length 1
 *          [5]          length 1
 *          [7,11]       length 2
 *          [13]          length 1
 *          [17,19,23]   length 3
 *          …
 *
 *  The function a(n) returns the length of the n‑th run (offset 1).
 *
 *  Implementation detail:
 *  - A simple prime sieve (trial division using a growing prime list)
 *    is used to generate primes in increasing order.
 *  - While generating primes we keep track of the current residue
 *    (prime % 4) and the length of the current run.  When the residue
 *    changes, the current run is finished and its length is stored in
 *    a dynamic array run_cache[].  The array is grown as needed.
 *  - The function ensures that run_cache contains at least n runs
 *    before returning run_cache[n‑1].
 *
 *  The code is valid C11 and C++17.  For the big‑integer build,
 *  seq_t represents an arbitrary‑precision integer class.  The code
 *  uses malloc/realloc, so explicit casts from void* are required for
 *  C++; the casts are harmless in C.
 *********************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic array of run lengths.  Allocated only when needed. */
static seq_t *run_cache = NULL;
static size_t  run_cache_len = 0; /* number of runs stored */
static size_t  run_cache_cap = 0; /* allocated capacity               */

/* Growing prime list used for trial division. */
static uint64_t *prime_list = NULL;
static size_t    prime_len = 0;
static size_t    prime_cap = 0;

/* State of the current run. */
static int  last_residue = -1;      /* residue of the current run (prime % 4) */
static size_t current_run_len = 0;   /* length of the current run */

/* Next integer candidate to test for primality.  After 2 we only
   consider odd numbers. */
static uint64_t next_candidate = 2;

/* Ensure run_cache can hold at least 'needed' elements.  */
static void grow_run_cache(size_t needed)
{
    if (run_cache_len >= needed)
        return;
    if (run_cache_cap == 0) {
        run_cache_cap = 64;
        run_cache = (seq_t *)malloc(run_cache_cap * sizeof(seq_t));
    }
    while (run_cache_cap < needed) {
        run_cache_cap <<= 1;
    }
    seq_t *tmp = (seq_t *)realloc(run_cache,
                                 run_cache_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    run_cache = tmp;
}

/* Ensure prime_list has room for one more entry.  */
static void grow_prime_list(void)
{
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    if (prime_len == prime_cap) {
        prime_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            prime_cap * sizeof(uint64_t));
        if (!tmp)
            exit(1);
        prime_list = tmp;
    }
}

/* Record a newly found prime.  */
static void process_prime(uint64_t p)
{
    grow_prime_list();
    prime_list[prime_len++] = p;

    int residue = p & 3;                     /* p % 4 */
    if (last_residue == -1) {                /* first prime (2) */
        last_residue   = residue;
        current_run_len = 1;
    } else if (residue == last_residue) {
        current_run_len++;                   /* extend current run */
    } else {                                /* run ends here */
        grow_run_cache(run_cache_len + 1);
        run_cache[run_cache_len++] = (seq_t)current_run_len;
        last_residue   = residue;
        current_run_len = 1;
    }
}

/* Generate the next prime after the current list.  */
static void generate_next_prime(void)
{
    /* First call: produce prime 2 */
    if (prime_len == 0) {
        process_prime(2);
        next_candidate = 3;   /* next odd number */
        return;
    }

    uint64_t p = next_candidate;
    if ((p & 1) == 0)  /* skip even numbers */
        p++;
    for (;;) {
        bool composite = false;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t q = prime_list[i];
            if (q * q > p)
                break;
            if (p % q == 0) {
                composite = true;
                break;
            }
        }
        if (!composite) {
            process_prime(p);
            next_candidate = p + 2; /* skip even, next odd */
            return;
        }
        p += 2;
    }
}

/* Ensure that at least 'n' runs have been recorded.  */
static void ensure_n_runs(idx_t n)
{
    while ((idx_t)run_cache_len < n)
        generate_next_prime();
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is irrelevant */
        return 0;
    ensure_n_runs(n);
    return run_cache[n - 1];
}
