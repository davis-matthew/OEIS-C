/*
 * A107817  Slowest increasing sequence where 2 consecutive integers sum up to
 *          a prime.
 * https://oeis.org/A107817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = smallest prime p > 2*a(n) minus a(n)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n+1) = p - a(n) where p = min{ prime | p > 2*a(n) } */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Very simple deterministic primality test for 64‑bit numbers.
   Works fast for numbers up to a few hundred thousand. */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

/* Static state to store computed terms. */
static seq_t     *seq   = NULL;
static idx_t     size  = 0;
static idx_t     cap   = 0;

/* Return a(n) – the n‑th term of the sequence (a(0)=0). */
seq_t a(idx_t n)
{
    /* Initialise the table with the first term if not already done. */
    if (size == 0) {
        cap  = 16;
        seq  = (seq_t*)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)0;
        size  = 1;
    }

    /* Generate terms up to the required index. */
    while (size <= n) {
        seq_t cur     = seq[size - 1];
        uint64_t cur64 = (uint64_t)cur;        /* cur is small enough for 64 bits */
        uint64_t cand  = cur64 * 2 + 1;         /* smallest odd > 2*cur */

        while (!is_prime_simple(cand))
            cand++;

        seq_t next = (seq_t)(cand - cur64);

        /* Resize table if needed. */
        if (size >= cap) {
            cap *= 2;
            seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
        }
        seq[size++] = next;
    }

    return seq[n];
}
