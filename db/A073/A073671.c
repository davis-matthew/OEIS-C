/*
 * A073671  Rearrangement of odd numbers such that sum of any three
 *          consecutive terms is prime.
 * https://oeis.org/A073671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest unused odd integer k such that a(n‑2) +
 *            a(n‑1) + k is prime. With a(1)=1, a(2)=3 this greedy
 *            construction yields OEIS A073671
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9583 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    /* static state for memoised construction */
    static bool initialized = false;
    static seq_t *seq      = NULL;     /* array of sequence values     */
    static size_t seq_cap  = 0;        /* capacity of seq array        */
    static size_t seq_len  = 0;        /* number of values stored      */
    static unsigned char *used = NULL; /* 0 = unused, 1 = used (by odd number) */
    static size_t used_size = 0;       /* size of used array in elements */

    /* initialise on first invocation */
    if (!initialized) {
        seq_cap   = 16;
        seq_len   = 2;
        seq       = mem_new(seq_t, seq_cap);
        if (!seq) abort();

        seq[0] = (seq_t)1;
        seq[1] = (seq_t)3;

        used_size = 8;                     /* covers indices 0..7 */
        used      = (unsigned char*)calloc(used_size, sizeof(unsigned char));
        if (!used) abort();
        used[1] = 1;   /* number 1 used */
        used[3] = 1;   /* number 3 used */

        initialized = true;
    }

    /* grow the sequence until the requested index is available */
    while (seq_len < n) {
        seq_t last1 = seq[seq_len - 2];
        seq_t last2 = seq[seq_len - 1];

        /* find the smallest unused odd number making the sum prime */
        idx_t cand = 1;            /* will be incremented to 3 first */
        seq_t candidate = 0;
        while (1) {
            cand += 2;                                 /* next odd number */

            /* ensure the used array is large enough */
            if ((size_t)cand >= used_size) {
                size_t new_size = used_size ? used_size << 1 : 8;
                while (new_size <= (size_t)cand) new_size <<= 1;
                unsigned char *tmp = (unsigned char*)realloc(used, new_size * sizeof(unsigned char));
                if (!tmp) abort();
                /* zero the newly added elements */
                for (size_t i = used_size; i < new_size; ++i) tmp[i] = 0;
                used = tmp;
                used_size = new_size;
            }

            if (used[(size_t)cand]) continue;          /* already used */

            candidate = (seq_t)cand;
            seq_t sum = last1 + last2 + candidate;
            if (is_prime_big(sum)) break;             /* found suitable candidate */
        }

        /* grow the sequence array if needed, before appending */
        if (seq_len >= seq_cap) {
            seq_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, seq, seq_cap);
            if (!tmp) abort();
            seq = tmp;
        }

        seq[seq_len++] = candidate;   /* append the new term */
        used[(size_t)cand] = 1;       /* mark it as used */
    }

    return seq[n - 1];
}
