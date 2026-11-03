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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
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
#include "../utils/is_prime_seq.h"
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
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
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
        if (is_prime_seq(cand)) {
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
