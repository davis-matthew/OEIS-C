/*
 * A205710  (prime(k)-prime(j))/8, where the pairs (k,j) are given by A205705
 *          and A205706.
 * https://oeis.org/A205710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(k)-prime(j))/8, where the pairs (k,j) are given by
 *            OEIS sequences A205705 (k-values) and A205706 (j-values), i.e.
 *            for each k in increasing order, all j < k with
 *            (prime(k)-prime(j)) divisible by 8 are taken in increasing order
 *            of j
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static seq_t *seq = NULL;        /* cached sequence values */
static size_t seq_cap = 0;       /* current capacity of seq array   */
static size_t seq_len = 0;       /* number of computed terms        */

/* current position in the enumeration */
static idx_t cur_k = 2;          /* next prime index k (>=2)       */
static idx_t cur_j = 1;          /* next prime index j (for cur_k) */

/* Return the next term of the sequence, advancing cur_k/cur_j.
   The function assumes cur_k and cur_j are initialised. */
static seq_t next_term(void)
{
    while (true) {
        /* Scan j from cur_j up to cur_k-1. */
        for (; cur_j < cur_k; ++cur_j) {
            seq_t diff = (seq_t)get_prime(cur_k) - (seq_t)get_prime(cur_j);
            if ((diff & 7u) == 0) {              /* diff % 8 == 0 */
                ++cur_j;                        /* next j for this k */
                return diff >> 3;                /* (prime(k)-prime(j))/8 */
            }
        }
        /* All j for current k examined; move to next k. */
        ++cur_k;
        cur_j = 1;
    }
}

/* Ensure that seq contains at least n terms. */
static void ensure_seq_len(idx_t n)
{
    if (n <= 0) return;
    while (seq_len < (size_t)n) {
        seq_t val = next_term();                  /* produce next value */
        if (seq_len >= seq_cap) {                 /* grow storage */
            seq_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
            seq = mem_resize(seq_t, seq, seq_cap);
            if (!seq) exit(EXIT_FAILURE);
        }
        seq[seq_len++] = val;                     /* store value */
    }
}

/* External function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined below offset */
    ensure_seq_len(n);
    return seq[n - 1];
}
