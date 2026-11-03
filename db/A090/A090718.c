/*
 * A090718  Let p run through the primes; write p in base 10 and then
 *          interpret it in base 128 getting a number q; if q is prime then
 *          adjoin q to the sequence.
 * https://oeis.org/A090718
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth number (n ≥ 2) obtained by taking each prime p
 *            in increasing order, writing p in base 10 and interpreting the
 *            same decimal digits as a base 128 integer q; if q is prime, q is
 *            added to the sequence. The sequence starts at a(2)=2 (offset 2)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 898 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* For indices below the offset the return value is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* Static cache of the sequence values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t size = 0;          /* number of terms currently stored */

    /* Static generator state for primes. */
    static uint64_t prime_candidate = 2;   /* current candidate for is_prime_small */

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        size = 0;
    }

    /* Generate terms until we have at least (n‑1) terms (indices 0…n‑2). */
    size_t needed = (size_t)(n - 1);
    while (size < needed) {
        /* Find next prime p. */
        uint64_t p;
        if (prime_candidate == 2) {
            p = 2;
            prime_candidate = 3;
        } else {
            while (!is_prime_seq((idx_t)prime_candidate))
                prime_candidate += 2;        /* skip even numbers */
            p = prime_candidate;
            prime_candidate += 2;
        }

        /* Convert decimal digits of p into base‑128 integer q. */
        uint8_t digits[20];
        size_t len = 0;
        uint64_t tmp = p;
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        if (len == 0) digits[len++] = 0;   /* never happens for primes */

        seq_t q = 0;
        for (size_t i = len; i-- > 0; ) {
            q = q * (seq_t)128 + (seq_t)digits[i];
        }

        /* If q is prime, store it. */
        if (is_prime_big(q)) {
            if (size == cap) {
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[size++] = q;
        }
    }

    /* a(2) is the first element of cache, so return cache[n‑2]. */
    return cache[n - 2];
}
