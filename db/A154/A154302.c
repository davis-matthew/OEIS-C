/*
 * A154302  Primes of the form (1+2+...+m)/87 = A000217(m)/87.
 * https://oeis.org/A154302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, bref
 * Formula:   a(n) = the n‑th prime of the form T(m)/87, where T(m)=m(m+1)/2
 *            that is p = m(m+1)/174. Only m ≡ 0,29,57,86 (mod 87) yield an
 *            integer
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
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

/* cache of found primes */
static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t last_m_checked = 0;

/* append a prime to the cache, growing it if necessary */
static void add_prime_to_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) exit(1);          /* allocation failed – terminate */
        prime_cache = tmp;
        cache_cap = new_cap;
    }
    prime_cache[cache_size++] = val;
}

/* return the n‑th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0 but safe */

    while (cache_size < (size_t)n) {
        last_m_checked++;                     /* next candidate m */
        uint64_t m = last_m_checked;
        uint64_t r = m % 87;

        /* only m with residues 0,29,57,86 give an integer result */
        if (r != 0 && r != 29 && r != 57 && r != 86) continue;

        seq_t mseq = (seq_t)m;
        seq_t p = (mseq * (mseq + 1)) / (seq_t)174;  /* p = m(m+1)/174 */

        if (is_prime_big(p)) add_prime_to_cache(p);
    }

    return prime_cache[n - 1];
}
