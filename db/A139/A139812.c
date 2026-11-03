/*
 * A139812  Primes prime(k) such that prime(k)^2 > prime(k-2)*prime(k+2).
 * https://oeis.org/A139812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 3498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9993 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A139812: prime(k) such that prime(k)^2 > prime(k-2)*prime(k+2) */
#include <stdlib.h>
#include <stdbool.h>
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

static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t  *primes  = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t  *qual    = NULL;
    static size_t qual_cnt  = 0, qual_cap  = 0;
    static idx_t prime_idx  = 0; /* index into get_prime (1‑based) */

    while (qual_cnt < (size_t)n) {
        /* generate next prime */
        ++prime_idx;
        uint32_t pk_val = get_prime(prime_idx);
        seq_t pk = (seq_t)pk_val;

        /* store prime */
        grow_seq(&primes, &primes_cap, primes_cnt + 1);
        primes[primes_cnt++] = pk;

        /* check condition once we have at least five primes (indices 1‑5) */
        if (primes_cnt >= 5) {
            /* k = primes_cnt - 3  (since primes[primes_cnt-1] is p_{k+2}) */
            seq_t p_k   = primes[primes_cnt - 3];
            seq_t p_k_2 = primes[primes_cnt - 5];
            seq_t p_kp2 = primes[primes_cnt - 1];
            seq_t left  = p_k * p_k;
            seq_t right = p_k_2 * p_kp2;
            if (left > right) {
                grow_seq(&qual, &qual_cap, qual_cnt + 1);
                qual[qual_cnt++] = p_k;
            }
        }
    }
    return qual[n - 1];
}
