/*
 * A048677  Concatenation of first n palindromic primes.
 * https://oeis.org/A048677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: concatenation of the first n palindromic primes */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* multiply x by 10^e; e is small (≤20 for our use case) */
static seq_t pow10_small(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* number of decimal digits of x (x > 0) */
static int digit_len(seq_t x)
{
    int len = 0;
    while (x > 0) {
        ++len;
        x /= 10;
    }
    return len > 0 ? len : 1;
}

/* generate next palindrome (not necessarily prime).  State is kept static. */
static seq_t next_palindrome(void)
{
    /* static state for the palindrome generator */
    static int  plen  = 1;            /* current palindrome length   */
    static uint64_t start = 1;       /* lower bound of current prefix  */
    static uint64_t end   = 9;       /* upper bound of current prefix  */
    static uint64_t cur   = 1;       /* current prefix value          */

    /* build palindrome from current prefix */
    seq_t pal = (seq_t)cur;
    uint64_t tmp = (plen & 1) ? cur / 10 : cur;   /* skip middle digit for odd length */
    while (tmp > 0) {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* advance prefix; if exceeded, increase length */
    ++cur;
    if (cur > end) {
        ++plen;
        int half = (plen + 1) / 2;
        start = 1;
        for (int i = 1; i < half; ++i)
            start *= 10;
        end   = start * 10 - 1;
        cur   = start;
    }
    return pal;
}

/* concatenate prev with p: prev * 10^d + p  */
static seq_t concat_prev_with(seq_t prev, seq_t p)
{
    seq_t pow = pow10_small((idx_t)digit_len(p));
    return prev * pow + p;
}

/* main function computing a(n) */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;     /* cache of computed terms */
    static idx_t   cap    = 0;        /* cache capacity */
    static idx_t   len    = 0;        /* number of terms cached */

    /* Ensure we have at least n terms computed */
    while (len < n) {
        /* find next palindromic prime */
        seq_t p;
        do {
            p = next_palindrome();
        } while (!is_prime_big(p));

        seq_t cur;
        if (len == 0)            /* first term */
            cur = p;
        else
            cur = concat_prev_with(cache[len - 1], p);

        /* grow cache if necessary */
        if (len >= cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* memory allocation failed; terminate */
                exit(1);
            }
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = cur;
    }
    return cache[n - 1];
}
