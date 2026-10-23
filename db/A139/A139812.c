/*
 * A139812  Primes prime(k) such that prime(k)^2 > prime(k-2)*prime(k+2).
 * https://oeis.org/A139812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 3498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A139812: prime(k) such that prime(k)^2 > prime(k-2)*prime(k+2) */
#include <stdlib.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */

static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
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
