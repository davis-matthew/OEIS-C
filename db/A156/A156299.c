/*
 * A156299  Primes where the first digit is greater than any other digit.
 * https://oeis.org/A156299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose first digit is strictly greater than
 *            any other digit
 * Range:     u8 n <= 10, u16 n <= 444, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 961 (2026-11-03)
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static bool is_valid(seq_t p) {
    /* A prime must have at least two digits and first digit > all other digits. */
    if (p < 10) return false;

    /* Extract most significant digit. */
    seq_t msd = p;
    while (msd >= 10)
        msd /= 10;

    /* Scan all digits except the most significant one. */
    seq_t tmp = p;
    while (tmp > 0) {
        if (tmp == msd) {          /* reached the most significant digit – skip it */
            tmp /= 10;
            continue;
        }
        seq_t d = tmp % 10;
        if (d >= msd)             /* any other digit not smaller */
            return false;
        tmp /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *seq_arr = NULL;   /* cached sequence */
    static size_t seq_len = 0;      /* number of elements in cache */
    static size_t seq_cap = 0;      /* capacity of cache */
    static seq_t current = 2;       /* candidate prime for next check */

    while (seq_len < n) {
        if (is_prime_big(current) && is_valid(current)) {
            append_candidate(&seq_arr, &seq_len, &seq_cap, current);
        }
        current += 1;
    }
    return seq_arr[n - 1];
}
