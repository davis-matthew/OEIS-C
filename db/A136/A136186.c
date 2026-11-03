/*
 * A136186  Primes whose decimal and binary reversal are both prime.
 * https://oeis.org/A136186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 236, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 961 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A136186 – Primes whose decimal and binary reversals are both prime.
 *  The function finds successive primes and retains those for which
 *  both rev_decimal(p) and reverse_binary(p) are prime.  The nth such
 *  prime (n ≥ 1) is returned.  Results are cached for repeated calls.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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

/* reverse the binary representation of n */
static seq_t reverse_binary(seq_t n)
{
    seq_t rev = 0;
    while (n != 0) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return rev;
}

static seq_t *cache = NULL;          /* stored qualifying primes */
static size_t cap   = 0;              /* allocated capacity of cache */
static size_t len   = 0;              /* current number of stored primes */
static seq_t last_checked = 1;       /* last prime examined */

seq_t a(idx_t n)
{
    size_t needed = (size_t)n;

    /* generate terms until we have at least 'needed' of them */
    while (len < needed) {
        /* find next prime after last_checked */
        do {
            last_checked++;
        } while (!is_prime_big(last_checked));

        seq_t prime = last_checked;

        seq_t rev_dec = reverse_digits(prime);
        if (!is_prime_big(rev_dec))
            continue;

        seq_t rev_bin = reverse_binary(prime);
        if (!is_prime_big(rev_bin))
            continue;

        /* store qualifying prime */
        if (len == cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) abort();        /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }
        cache[len++] = prime;
    }

    return cache[needed - 1];
}
