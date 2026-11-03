/*
 * A202265  Nonprime numbers in which all substrings and reversal substrings
 *          are nonprimes.
 * https://oeis.org/A202265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by searching for the n‑th nonprime number whose
 *            every substring and every reverse substring is nonprime. The
 *            search is performed in increasing numeric order by generating
 *            all valid numbers of length 1, then all of length 2, and so on.
 *            The implementation uses depth‑first search for e...
 * Range:     u8 n <= 28, u16 n <= 469, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 74 terms through n = 1950 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Maximum number of digits we are willing to handle.
   For the typical test cases (n ≤ 10000) the 10000‑th term
   is far below 200 digits, so this is safe for any
   built seq_t that can hold that many digits. */
#define MAX_DEPTH 200

/* Allowed digits (most‑significant first).  The digit '0' is
   permitted only after the first position. */
static const seq_t ALLOWED[6] = {0, 1, 4, 6, 8, 9};

/* Global state for the DFS.  All arrays are indexed by depth
   and suffix length (1 ≤ len ≤ depth).  The entries at
   depth 0 are not used. */
static seq_t S[MAX_DEPTH + 1][MAX_DEPTH + 1];
static seq_t R[MAX_DEPTH + 1][MAX_DEPTH + 1];
static seq_t POW10[MAX_DEPTH + 1];

/* Result for the current query. */
static seq_t  ANSWER;
static idx_t  TARGET_N;
static bool   FOUND;
static idx_t  SEEN /* count of terms found so far, including the 0 term */;

/* Record a candidate number that satisfies the property.
   Once the TARGET_N'th term is found, store it in ANSWER. */
static void record_value(seq_t v)
{
    SEEN++;
    if (SEEN == TARGET_N) {
        ANSWER = v;
        FOUND = true;
    }
}

/* Depth‑first search for numbers of exact length L.
   depth   – number of digits already chosen (0 … L-1)
   val     – numeric value of those digits */
static void dfs_len(idx_t target_len, int depth, seq_t val)
{
    if (FOUND) return;

    if (depth == target_len) {
        record_value(val);
        return;
    }

    /* Iterate over all allowed digits.  Skip leading zero. */
    for (int i = 0; i < 6 && !FOUND; ++i) {
        seq_t d = ALLOWED[i];
        if (depth == 0 && d == 0) continue;

        seq_t new_val = val * (seq_t)10 + d;

        /* Single‑digit substring must be nonprime. */
        if (is_prime_big(d)) continue;

        /* Prepare arrays for the new depth. */
        seq_t *S_cur = S[depth + 1];
        seq_t *R_cur = R[depth + 1];

        S_cur[1] = d;
        R_cur[1] = d;

        /* Pow10[depth+1] is needed for deeper levels. */
        POW10[depth + 1] = POW10[depth] * (seq_t)10;

        bool ok = true;

        /* Extend all existing suffixes. */
        for (int l = 1; l <= depth; ++l) {
            seq_t new_suf = S[depth][l] * (seq_t)10 + d;
            if (is_prime_big(new_suf)) { ok = false; break; }
            seq_t new_rev = d * POW10[l] + R[depth][l];
            if (is_prime_big(new_rev)) { ok = false; break; }
            S_cur[l + 1] = new_suf;
            R_cur[l + 1] = new_rev;
        }

        if (!ok) continue;

        dfs_len(target_len, depth + 1, new_val);
    }
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;  /* a(1) = 0 */

    TARGET_N = n;
    FOUND    = false;
    ANSWER   = (seq_t)0;
    SEEN     = (idx_t)1;                 /* zero counted as first term */

    POW10[0] = (seq_t)1;

    for (int len = 1; len <= MAX_DEPTH && !FOUND; ++len) {
        dfs_len((idx_t)len, 0, (seq_t)0);
    }

    return ANSWER;
}
