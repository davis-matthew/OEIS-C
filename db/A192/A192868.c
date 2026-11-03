/*
 * A192868  Thin numbers: odd numbers k > 1 such that k+1 is a prime times a
 *          power of two.
 * https://oeis.org/A192868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 7488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9973 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Thin numbers: odd k > 1 such that k+1 = prime × 2^m (m ≥ 1) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Static cache of thin numbers.
   thin_cache holds the found thin numbers,
   thin_len is the number stored,
   thin_cap is the allocated capacity,
   thin_next is the next odd number to be examined. */
static seq_t *thin_cache = NULL;
static size_t thin_cap   = 0;
static size_t thin_len   = 0;
static idx_t  thin_next  = 3;

/* Test whether an odd number k > 1 is thin.
   k+1 must be of the form prime × 2^m.  The odd part is
   extracted by right‑shifting until it becomes odd.  If this
   odd part is 1, *k+1* is a power of two, hence thin because the
   prime factor is 2.  Otherwise the odd part must be prime. */
static bool isThin(seq_t k)
{
    seq_t x = k + 1;
    while ((x & 1) == 0)          /* strip all factors of two */
        x >>= 1;
    if (x == 1)                   /* k+1 is a power of two -> thin */
        return true;
    return is_prime_big(x);       /* remaining odd part must be prime */
}

/* Make sure at least 'target' thin numbers have been cached. */
static void ensure_thin(idx_t target)
{
    if (thin_len >= target)
        return;

    for (idx_t k = thin_next; ; k += 2) {
        if (isThin((seq_t)k)) {
            if (thin_len == thin_cap) {
                size_t new_cap = thin_cap ? thin_cap << 1 : 16;
                grow_seq(&thin_cache, &thin_cap, new_cap);
            }
            thin_cache[thin_len++] = (seq_t)k;
            if (thin_len == target) {
                thin_next = k + 2;   /* next candidate after this one */
                return;
            }
        }
    }
}

/* Public function required by the problem statement.
   a(1) is the first thin number, a(2) the second, and so on.
   For n == 0 this returns 0 (unused by the OEIS file). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n < 1, harmless */
    ensure_thin(n);
    return thin_cache[n - 1];
}
