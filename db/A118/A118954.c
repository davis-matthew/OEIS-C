/*
 * A118954  Numbers that cannot be written as 2^k + prime.
 * https://oeis.org/A118954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers that cannot be written as 2^k + prime (OEIS
 *            A118954)
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9372 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *seq_cache = NULL;      /* cached non‑representable numbers */
static size_t seq_len = 0;           /* number of terms stored */
static size_t seq_cap = 0;           /* current array capacity */
static idx_t last_checked = 0;       /* last integer examined */

/* Append a new term to the cache, enlarging the array if necessary */
static void append_cache(seq_t val) {
    if (seq_len == seq_cap) {
        /* Ensure capacity for at least `seq_len + 1` elements. */
        grow_seq(&seq_cache, &seq_cap, seq_len + 1);
    }
    seq_cache[seq_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1; behaviour for n == 0 is undefined. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Next integer to test */
        last_checked++;
        seq_t m = (seq_t)last_checked;

        /* Check if m can be expressed as 2^k + prime. */
        bool representable = false;
        seq_t power = (seq_t)1;              /* 2^0 */
        while (power <= m) {
            seq_t p = m - power;
            if (p >= (seq_t)2 && is_prime_big(p)) {
                representable = true;
                break;
            }
            power <<= 1;                     /* next power of two */
        }

        /* If not representable, it belongs to the sequence. */
        if (!representable) {
            append_cache(m);
        }
    }

    /* Return the (n-1)th term (indices start at 1). */
    return seq_cache[n - 1];
}
