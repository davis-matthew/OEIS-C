/*
 * A030540  Largest n-digit norep emirp.
 * https://oeis.org/A030540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = largest n‑digit distinct‑digit emirp (0 if none)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Depth‑first search generates n‑digit numbers with distinct digits in
   descending order; the first that satisfies both primality tests is
   returned.  The search uses the supplied primality routine is_prime_big. */

#include <stdbool.h>
#include <stdint.h>
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

/* Recursive search helper. */
static bool dfs_emirp(int pos, int n, int *dig, bool *used, seq_t *res)
{
    if (pos == n) {
        seq_t num = 0;
        for (int i = 0; i < n; ++i)     /* build the number */
            num = num * 10 + (seq_t)dig[i];
        seq_t rev = 0;                   /* its decimal reversal */
        for (int i = n - 1; i >= 0; --i)
            rev = rev * 10 + (seq_t)dig[i];
        if (is_prime_big(num) && is_prime_big(rev)) {
            *res = num;
            return true;
        }
        return false;
    }
    for (int d = 9; d >= 0; --d) {
        if (pos == 0 && d == 0) continue;    /* no leading zero */
        if (!used[d]) {
            used[d] = true;
            dig[pos] = d;
            if (dfs_emirp(pos + 1, n, dig, used, res))
                return true;
            used[d] = false;
        }
    }
    return false;
}

/* Compute the largest n‑digit distinct‑digit emirp. */
static seq_t solve_emirp(idx_t n)
{
    if (n == 0 || n > 10) return 0;           /* no such number for other n */
    int digits[10];
    bool used[10] = { false };
    seq_t result = 0;
    dfs_emirp(0, (int)n, digits, used, &result);
    return result;
}

/* Public entry point for the OEIS algorithm. */
seq_t a(idx_t n)
{
    static seq_t cache[11] = {0};
    static bool done[11] = {false};

    if (n == 0 || n > 10) return 0;           /* undefined for these n */
    if (!done[(int)n]) {
        cache[(int)n] = solve_emirp(n);
        done[(int)n] = true;
    }
    return cache[(int)n];
}
