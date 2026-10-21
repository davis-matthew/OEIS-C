/*
 * A073161  Nested floor product of n and fractions (p_k + 1)/p_k where p_k is
 *          k-th prime.
 * https://oeis.org/A073161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(a(n-1)*(p1+1)/p1), floor(a(n-2)*(p2+1)/p2), ...
 *            with p_k the k‑th prime. The computation stops once the value no
 *            longer changes. Indexing starts at 1, a(1)=1
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime generation utilities ---------- */
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;
static idx_t  next_candidate = 2;  /* next odd to test, starting from 3
                                     (2 already in the list). */

/* Initialize the prime table with 2. */
static void init_prime_table(void)
{
    if (prime_cnt == 0) {
        prime_cap = 8;
        prime_list = (idx_t*)calloc(prime_cap, sizeof(idx_t));
        prime_list[0] = (idx_t)2;
        prime_cnt = 1;
        next_candidate = 3;   /* next odd to test after 2 */
    }
}

/* Generate the next prime, append it to prime_list, and return it. */
static idx_t generate_next_prime(void)
{
    init_prime_table();

    for (;;) {
        /* Skip even numbers. */
        if ((next_candidate & 1u) == 0) {
            ++next_candidate;
            continue;
        }

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > next_candidate / p) break;          /* p*p > candidate */
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime) {
            ++next_candidate;
            continue;
        }

        /* Store new prime if needed. */
        if (prime_cnt == prime_cap) {
            prime_cap = prime_cap ? prime_cap * 2 : 8;
            prime_list = (idx_t*)realloc(prime_list, prime_cap * sizeof(idx_t));
        }
        prime_list[prime_cnt++] = next_candidate;
        ++next_candidate;   /* advance to next candidate for future calls */
        return prime_list[prime_cnt - 1];
    }
}

/* ---------- Sequence implementation ---------- */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;

    /* iterate over primes starting from the first (2). */
    idx_t pi = 0;
    while (1) {
        /* Ensure we have a prime available. */
        if (pi >= prime_cnt) {
            generate_next_prime();   /* expands prime_list as needed */
        }

        idx_t p = prime_list[pi++];
        seq_t new_val = (val * ((seq_t)p + (seq_t)1)) / (seq_t)p;
        if (new_val == val)
            break;
        val = new_val;
    }

    return val;
}
