/*
 * A075996  Primes of the form 2 + 2^n + 3^n.
 * https://oeis.org/A075996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form 2 + 2^k + 3^k (k starting at 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static inline void append_prime(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 128 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *arr, newcap);
        if (!tmp) abort();            /* Out of memory – give up. */
        *arr = tmp;
        *cap = newcap;
    }
    (*arr)[(*len)++] = val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static storage for cache and state */
    static seq_t *prime_cache = NULL;
    static size_t prime_len  = 0;
    static size_t prime_cap  = 0;
    static seq_t  cur_p2 = 0;   /* 2^k */
    static seq_t  cur_p3 = 0;   /* 3^k */
    static idx_t  k       = 0;   /* current exponent (1‑based) */
    static int    init    = 0;

    /* One‑time initialization */
    if (!init) {
        prime_cap = 128;
        prime_cache = mem_new(seq_t, prime_cap);
        if (!prime_cache) abort();
        cur_p2 = 2;          /* 2^1 */
        cur_p3 = 3;          /* 3^1 */
        k      = 1;
        init = 1;
    }

    /* Generate more primes until we have at least n of them */
    while (prime_len < (size_t)n) {
        /* val = 2 + 2^k + 3^k */
        seq_t val = cur_p2 + cur_p3 + (seq_t)2;

        if (is_prime_big(val))
            append_prime(&prime_cache, &prime_len, &prime_cap, val);

        /* Prepare next exponent, checking for overflow on fixed‑width types */
        /* multiplications */
        seq_t new_p2 = cur_p2 + cur_p2; /* cur_p2 * 2 */
        seq_t new_p3 = cur_p3 + cur_p3; /* cur_p3 * 2 */
        if (new_p3 < cur_p3) new_p3 = cur_p3;           /* detect wrap */
        new_p3 = new_p3 + cur_p3;                       /* now multiply by 3 */
        if (new_p3 < cur_p3 || new_p3 < new_p3) break;  /* overflow */

        if (new_p2 < cur_p2) break; /* overflow for power of two */

        cur_p2 = new_p2;
        cur_p3 = new_p3;
        ++k;
    }

    if (prime_len < (size_t)n) return 0;  /* cannot compute more terms */

    return prime_cache[(size_t)n - 1];
}
