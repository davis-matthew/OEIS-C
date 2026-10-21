/*
 * A082444  Palindromic primes with middle digit 8.
 * https://oeis.org/A082444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 612, u64 n <= 4796,
 *            u128 n <= 4796, big any n
 * Helpers:   modpow_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate palindromic primes having the middle digit 8 by
 * iterating odd‑length palindromes with a fixed centre 8 and testing each
 * number for primality. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/modpow_u64.h"

/* ---------- small utilities ---------- */

/* power of ten, result type is seq_t */
static inline seq_t pow10(idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        r *= 10;
        e--;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; i++) {
        uint64_t p = small[i];
        if (n % p == 0)
            return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; i++) {
        uint64_t a = bases[i];
        if (a >= n)
            break;
        uint64_t x = modpow_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = (uint64_t)((unsigned __int128)x * x % n);
            if (x == n - 1) {
                comp = false;
                break;
            }
        }
        if (comp)
            return false;
    }
    return true;
}

/* ---------- memoisation of palindromic primes ---------- */

static seq_t *pal_list    = NULL;   /* stored primes */
static size_t pal_len     = 0;      /* number of terms stored */
static size_t pal_cap     = 0;      /* allocated capacity */

/* increase storage if needed */
static void ensure_pal_cap(size_t need)
{
    if (pal_cap >= need)
        return;
    size_t new_cap = pal_cap ? pal_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(pal_list, new_cap * sizeof(seq_t));
    pal_list = tmp;
    pal_cap = new_cap;
}

/* ---------- generation state ---------- */

static idx_t  gen_len    = 3;          /* current odd length of palindrome */
static uint64_t gen_start = 1;        /* first prefix for current length */
static uint64_t gen_end   = 9;        /* last prefix for current length */
static uint64_t gen_cur   = 1;        /* next prefix to try */

/* generate next palindromic prime with middle digit 8 */
static seq_t next_palprime(void)
{
    while (1) {
        idx_t k = (gen_len - 1) / 2;          /* digits in left part */
        seq_t left = gen_cur;                 /* left half */

        /* reverse of left half */
        uint64_t t = gen_cur;
        seq_t rev = 0;
        while (t != 0) {
            rev = rev * 10 + (t % 10);
            t /= 10;
        }

        /* construct full palindrome: left | 8 | reverse(left) */
        seq_t val = left * pow10(k + 1) + (seq_t)8 * pow10(k) + rev;

        /* advance generation state */
        gen_cur++;
        if (gen_cur > gen_end) {
            gen_len += 2;
            k = (gen_len - 1) / 2;
            gen_start = (uint64_t)pow10(k - 1);
            gen_end   = (uint64_t)pow10(k) - 1;
            gen_cur   = gen_start;
        }

        /* test for primality (all values fit in 64‑bit for n ≤ 10000) */
        if (is_prime_u64((uint64_t)val))
            return val;
    }
}

/* ---------- public interface ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, defensive fallback */

    while (pal_len < n) {
        ensure_pal_cap(pal_len + 1);
        pal_list[pal_len] = next_palprime();
        pal_len++;
    }
    return pal_list[n - 1];
}
