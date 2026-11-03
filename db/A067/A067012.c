/*
 * A067012  Absolute composites: every permutation of digits (dropping any
 *          leading zeros) is a composite number.
 * https://oeis.org/A067012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 4776, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9694 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: test integers >=4 incrementally; an absolute composite is
 * a composite integer whose every permutation of decimal digits (with any
 * leading zeros dropped) is also composite.  The function returns the
 * nth such integer (n>=1). */
#include <stdbool.h>
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

/* Back‑tracking over all distinct permutations of the multiset whose
 * digit counts are given in cnt[10].  The current number being built is
 * stored in cur.  If a permutation yields a value that is <4 or prime,
 * *bad_found is set. */
static void check_permutations(int remaining, int cnt[10], seq_t cur,
                               bool *bad_found)
{
    if (*bad_found) return;
    if (remaining == 0) {
        if (cur < 4 || is_prime_big(cur))
            *bad_found = true;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] == 0) continue;
        --cnt[d];
        check_permutations(remaining - 1, cnt, cur * 10 + d, bad_found);
        ++cnt[d];
        if (*bad_found) return;
    }
}

/* Return true iff x is an absolute composite. */
static bool is_absolute_composite(seq_t x)
{
    if (x < 4 || is_prime_big(x))
        return false;                     /* x itself must be composite */

    int cnt[10] = {0};
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        int d = (int)(t % 10);             /* convert to int for array index */
        ++cnt[d];
        t /= 10;
        ++len;
    }

    bool bad = false;
    check_permutations(len, cnt, 0, &bad);
    return !bad;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *list = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t last_checked = 3;         /* start at 4 */

    while (len < n) {
        seq_t val = ++last_checked;
        if (val < 4 || is_prime_big(val))
            continue;
        if (is_absolute_composite(val)) {
            if (len == cap) {
                cap = (cap == 0) ? 64 : cap * 2;
                list = mem_resize(seq_t, list, cap);
            }
            list[len++] = val;
        }
    }
    return list[n - 1];
}
