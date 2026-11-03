/*
 * A089987  Primes in the concatenation of consecutive numbers beginning with
 *          2.
 * https://oeis.org/A089987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089987: primes obtained by concatenating consecutive integers
   starting with 2.
   The function builds the concatenated integer incrementally, tests each
   new prefix for primality, and caches the primes in a static array so
   that repeated calls require no recomputation.  No closed formula is known
   for this sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    if (n == 0)
        return 0;                          /* offset 1 – not used */

    /* Static state retained across calls */
    static seq_t *primes = NULL;            /* cached primes                            */
    static size_t cap = 0;                  /* capacity of the array                    */
    static size_t count = 0;                /* number of primes stored                   */
    static idx_t cur_m = 1;                 /* last integer appended to the number       */
    static seq_t cur_val = 0;               /* current concatenated value                */

    /* Allocate initial array if first call */
    if (!primes) {
        cap = 128;
        primes = mem_new(seq_t, cap);
    }

    /* Ensure array can hold at least 'count+1' elements */
    if (count >= cap) {
        cap <<= 1;
        primes = mem_resize(seq_t, primes, cap);
    }

    /* Generate primes until we have at least n of them */
    while (count < n) {
        /* Extend the concatenation by the next integer */
        ++cur_m;
        int d = num_digits_idx(cur_m);                       /* number of decimal digits */
        seq_t pow10d = pow10((idx_t)d);
        cur_val = cur_val * pow10d + (seq_t)cur_m;

        /* Record the new prime if detected */
        if (is_prime_big(cur_val)) {
            if (count >= cap) {                      /* just in case                 */
                cap <<= 1;
                primes = mem_resize(seq_t, primes, cap);
            }
            primes[count++] = cur_val;
        }
    }

    return primes[n - 1];
}
