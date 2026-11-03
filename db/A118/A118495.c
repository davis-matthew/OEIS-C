/*
 * A118495  Chen primes written backwards.
 * https://oeis.org/A118495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chen primes written backwards: a(n) is the decimal reversal of the n‑th Chen prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"
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

static idx_t current_candidate = 2;
static seq_t *chen_rev = NULL;
static size_t chen_cnt = 0;
static size_t chen_cap = 0;

/* Reverse the decimal digits of an integer.  Result fits in seq_t. */
static seq_t rev_digits(idx_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (chen_cnt < n) {
        /* Find next prime p. */
        while (!is_prime_big((seq_t)current_candidate))
            current_candidate++;

        idx_t p = current_candidate;
        idx_t q = p + 2;

        bool chen = false;
        if (is_prime_big((seq_t)q)) {
            chen = true;
        } else {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64((uint64_t)q, primes, exps, &cnt);
            int total = 0;
            for (size_t i = 0; i < cnt; ++i)
                total += (int)exps[i];
            if (total == 2)
                chen = true;
        }

        if (chen) {
            seq_t rev = rev_digits(p);
            if (chen_cnt >= chen_cap)
                grow_seq(&chen_rev, &chen_cap, chen_cnt + 1);
            chen_rev[chen_cnt++] = rev;
        }

        current_candidate++;
    }

    return chen_rev[n - 1];
}
