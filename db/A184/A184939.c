/*
 * A184939  From the base sequence of the positive integers, keep the first
 *          two, remove the next three, keep the next five, remove the next
 *          seven, ..., block lengths determined by the prime numbers.
 * https://oeis.org/A184939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: keep blocks of consecutive integers of length prime numbers
 * starting with the first prime 2.  Blocks with odd index (1,3,5,…) are kept,
 * even-indexed blocks are discarded.  Return the nth kept integer (a(1)=1). */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the primes that we build on demand. */
static uint64_t *primes = NULL;   /* array of primes */
static size_t primes_cnt  = 0;    /* number of primes stored */
static size_t primes_cap  = 0;    /* capacity of the array */

/* Generate the next prime after primes[primes_cnt-1] and store it. */
static void generate_next_prime(void)
{
    uint64_t candidate = (primes_cnt == 0) ? 2ULL : primes[primes_cnt - 1] + 1ULL;
    for (;; ++candidate) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 8 : primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    new_cap * sizeof(uint64_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
            break;
        }
    }
}

/* Ensure that at least `need` primes are available. */
static void ensure_primes(size_t need)
{
    while (primes_cnt < need) generate_next_prime();
}

/* Return the nth element of sequence A184939.  The OEIS offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, but safe */

    idx_t kept_so_far  = 0;   /* number of kept values seen so far      */
    idx_t total_numbers = 0; /* total numbers processed (kept+discarded) */

    idx_t block = 1;         /* block index, starting from 1            */

    while (1) {
        /* Make sure the prime for this block exists. */
        ensure_primes((size_t)block);
        idx_t block_size = (idx_t)primes[block - 1]; /* length of this block */

        if (block & 1) {   /* keep block when block index is odd */
            idx_t kept_before = kept_so_far;
            kept_so_far += block_size;
            if (kept_so_far >= n) {
                /* The desired element is inside this block. */
                idx_t start_of_block = total_numbers + 1; /* first number in block */
                idx_t offset = n - kept_before;           /* 1‑based offset in block */
                return (seq_t)(start_of_block + offset - 1);
            }
        }

        total_numbers += block_size; /* update total processed numbers */
        ++block;                     /* go to the next block               */
    }
}
