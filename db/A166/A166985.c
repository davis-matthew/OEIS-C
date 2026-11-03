/*
 * A166985  Primes of the form phi(n)/2.
 * https://oeis.org/A166985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166985: primes of the form φ(n)/2.
   a(n) = the n‑th prime that can be written as φ(m)/2 for some integer m > 2.
   Additional description: we scan integers m starting at 3, compute
   φ(m) with calc_phi, divide by 2 (bit shift), and record the result
   whenever it is prime (checked by is_prime_big).  Cached results are
   stored so that calls may come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
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

static seq_t *prime_cache = NULL;   /* memoised primes */
static size_t cache_len = 0;        /* number of primes stored */
static size_t cache_cap = 0;        /* allocated capacity */
static idx_t last_checked_n = 2;    /* last m that was examined */

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp)
        abort();                    /* critical failure – cannot continue */
    prime_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 to signal error */
    if (n == 0)
        return 0;

    /* extend memoised cache until it contains at least n primes */
    while (cache_len < (size_t)n) {
        ++last_checked_n;                    /* next integer to test */
        seq_t phi = totient_seq((seq_t)last_checked_n);
        seq_t half = phi >> 1;               /* φ(n)/2 */
        if (is_prime_big(half)) {
            if (cache_len == cache_cap)
                grow_cache();
            prime_cache[cache_len++] = half;
        }
    }
    return prime_cache[n - 1];
}
