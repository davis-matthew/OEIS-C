/*
 * A089685  a={1,3,7,9} p[n]=Prime[n]*10+a[[1+Mod[n,4]]] a(m) = if p[n] is
 *          prime, then p[n].
 * https://oeis.org/A089685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime_k * 10 + d_k, where k is the index of the prime used
   and d_k cycles through {1,3,7,9} in the order 3,7,9,1,3,7,....
   The function generates primes sequentially and keeps a cache of the
   primes of this form so that arbitrary order of calls is answered
   efficiently.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(uint32_t) */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) */

/* static storage for primes and the sequence values */
static uint32_t *primes = NULL;   /* array of discovered primes          */
static size_t prime_cnt = 0;      /* number of primes stored             */
static size_t prime_cap = 0;      /* capacity of primes array            */

static seq_t *seq_cache = NULL;   /* array of values of the sequence    */
static size_t seq_len = 0;        /* number of sequence values stored   */
static size_t seq_cap = 0;        /* capacity of seq_cache              */

static uint32_t next_candidate = 3;   /* next integer to test for primality */

/* ensure capacity for more primes, grows to at least needed */
static void _ensure_prime_cap(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) return;   /* allocation failure – caller will get 0 */
    primes = tmp;
    prime_cap = new_cap;
}

/* ensure capacity for more sequence elements */
static void _ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* generate the next prime after the last one stored,
   updating primes[], prime_cnt and next_candidate. */
static uint32_t _next_prime(void)
{
    /* first prime is 2 */
    if (prime_cnt == 0) {
        _ensure_prime_cap(1);
        primes[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;
        return 2;
    }
    for (uint32_t cand = next_candidate; ; cand += 2) {
        if (is_prime_small(cand)) {
            _ensure_prime_cap(prime_cnt + 1);
            primes[prime_cnt++] = cand;
            next_candidate = cand + 2;
            return cand;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* generate the sequence until we have n elements */
    while (seq_len < n) {
        uint32_t p = _next_prime();           /* next prime p_k          */
        unsigned d = (unsigned)((prime_cnt) % 4);   /* cycle 0..3             */
        static const unsigned char digits[4] = {1, 3, 7, 9};
        seq_t candidate = (seq_t)p * 10 + digits[d];
        if (is_prime_big(candidate)) {
            _ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = candidate;
        }
    }

    return seq_cache[n - 1];
}
