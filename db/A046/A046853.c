/*
 * A046853  Denominator of sum of reciprocals of first n palindromic primes.
 * https://oeis.org/A046853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 94 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominator of the sum of the reciprocals of the first n
   palindromic primes. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
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

/* ---------- dynamic array helpers ---------- */
static void ensure_capacity(size_t *cap, size_t needed, seq_t **ptr) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = mem_resize(seq_t, *ptr, new_cap);
    if (!new_ptr) exit(1);
    *ptr = new_ptr;
    *cap = new_cap;
}

/* ---------- palindromic prime generator ---------- */
static idx_t  g_len      = 1;          /* current palindrome length (odd) */
static seq_t  g_half_min = 1;          /* minimal half for current length   */
static seq_t  g_half_max = 9;          /* maximal half for current length   */
static seq_t  g_half     = 1;          /* current half value                */
static bool   g_done_len1 = false;     /* finished generating 1‑digit pals  */

static seq_t next_pal_prime(void) {
    while (true) {
        /* Finished 1‑digit palindromes: return 11 and start length 3. */
        if (!g_done_len1 && g_len == 1 && g_half > g_half_max) {
            g_done_len1 = true;
            g_len = 3;
            g_half_min = (seq_t)10;
            g_half_max = (seq_t)99;
            g_half = g_half_min;
            return (seq_t)11;
        }

        /* If current length is finished, advance to next odd length. */
        if (g_half > g_half_max) {
            g_len += 2;
            g_half_min = g_half_min * (seq_t)10;
            g_half_max = g_half_max * (seq_t)10 + (seq_t)9;
            g_half = g_half_min;
            continue;
        }

        /* Build palindrome from current half. */
        seq_t pal = g_half;
        seq_t t = g_half / (seq_t)10;   /* skip middle digit for odd length */
        while (t > 0) {
            pal = pal * (seq_t)10 + (t % (seq_t)10);
            t = t / (seq_t)10;
        }
        g_half = g_half + (seq_t)1;

        if (is_prime_big(pal))
            return pal;
    }
}

/* ---------- denominator computation ---------- */
static seq_t *denoms     = NULL;
static size_t den_cap    = 0;
static size_t den_cnt    = 0;
static seq_t  num_last   = 0;
static seq_t  den_last   = 1;   /* start with 0/1 */

// Ensure that denominators up to index n (1‑based) are computed.
static void ensure_denoms(idx_t n) {
    if (den_cnt >= (size_t)n) return;

    ensure_capacity(&den_cap, (size_t)n, &denoms);

    while (den_cnt < (size_t)n) {
        seq_t p = next_pal_prime();

        // num_last/den_last + 1/p
        seq_t new_num = num_last * p + den_last;
        seq_t new_den = den_last * p;
        seq_t g       = gcd(new_num, new_den);
        num_last = new_num / g;
        den_last = new_den / g;

        denoms[den_cnt++] = den_last;
    }
}

/* ---------- public API ---------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_denoms(n);
    return denoms[n - 1];
}
