/*
 * A126096  Primes that are the arithmetic mean of four successive primes.
 * https://oeis.org/A126096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes the primes that are the arithmetic mean of four
 *            successive primes by scanning successive primes up to a fixed
 *            upper bound, collecting those whose mean is itself a prime
 * Range:     u8 none, u16 n <= 72, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 59 terms through n = 2803 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
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

seq_t a(idx_t n)
{
    /* static storage for the sequence values */
    static seq_t* seq_arr = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* primes array and related state */
    static uint64_t* primes_u64 = NULL;
    static size_t primes_cnt = 0;
    static idx_t cur_block_start = 0;
    static bool primes_ready = false;

    /* Prepare primes once with a generous upper bound. */
    if (!primes_ready) {
        /* 20 million is sufficient to cover at least 10 000 terms
           of the sequence while staying far below memory limits. */
        const idx_t LIMIT = 20000000;
        build_primes_upto(LIMIT, &primes_u64, &primes_cnt);
        primes_ready = true;
    }

    /* Generate sequence values until we have the n‑th term. */
    while (seq_len < (size_t)n &&
           cur_block_start <= primes_cnt - (idx_t)4)
    {
        /* Sum the four successive primes. The result fits in seq_t,
           because primes are ≤ 20 million. */
        seq_t sum =
            (seq_t)primes_u64[cur_block_start]     +
            (seq_t)primes_u64[cur_block_start + 1] +
            (seq_t)primes_u64[cur_block_start + 2] +
            (seq_t)primes_u64[cur_block_start + 3];

        /* The mean is an integer only if the sum is divisible by 4. */
        if ((sum & 3U) == 0) {
            seq_t mean = sum >> 2; /* divide by 4 */
            if (is_prime_big(mean)) {
                /* Grow the result array if necessary. */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? (seq_cap << 1) : 16;
                    seq_arr = mem_resize(seq_t, seq_arr, seq_cap);
                }
                seq_arr[seq_len++] = mean;
            }
        }

        ++cur_block_start;
    }

    /* The requested term is now available. */
    return seq_arr[n - 1];
}
