/*
 * A133957  Form the list of home primes A037274(c) for c composite, and sort
 *          into increasing order.
 * https://oeis.org/A133957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A133957 – sorted list of all primes that can be written
   as a concatenation of at least two primes in nondecreasing order.
   The code generates primes in increasing order, tests each prime
   for such a concatenation, and stores the qualifying primes in
   a static table (memoised).  The nth term is returned from this
   table.  No hard‑coded terms are used. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_seq.h"
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

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* --------------------------------------------------------------------- */
/*  Test whether a prime number p can be split into at least two primes
    that appear in nondecreasing numeric order.  The function receives
    the decimal representation of p as a character array *s of length
    *len.  It returns true if such a split exists.                     */
/* --------------------------------------------------------------------- */
static bool split_rec(const char *s, size_t pos, size_t len,
                      seq_t prev, int parts)
{
    if (pos == len) return parts >= 2;                /* reached end */
    if (s[pos] == '0') return false;                  /* leading zero not allowed */

    seq_t val = 0;
    for (size_t j = pos; j < len; ++j) {
        val = val * 10 + (seq_t)(s[j] - '0');
        /* enforce nondecreasing order */
        if ((parts == 0 || val >= prev) && is_prime_big(val)) {
            if (split_rec(s, j + 1, len, val, parts + 1))
                return true;
        }
    }
    return false;
}

static bool can_split_nondec(const char *s, size_t len)
{
    return split_rec(s, 0, len, 0, 0);
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *home = NULL;
    static size_t home_count = 0;
    static size_t home_cap   = 0;
    static idx_t  candidate  = 2;          /* numbers to test, start at 2 */

    /* Allocate the home array lazily. */
    if (!home) {
        home_cap = 8;
        home = mem_new(seq_t, home_cap);
        if (!home) return 0;               /* allocation failure */
    }

    /* Find new qualifying primes until we have at least n of them. */
    while (home_count < n) {
        seq_t cand = (seq_t)candidate;
        if (is_prime_seq(cand)) {          /* test only primes */
            char buf[64];
            size_t len;
            seq_to_str(cand, buf, &len);

            if (can_split_nondec(buf, len)) {
                /* Store prime if it satisfies the split condition. */
                if (home_count == home_cap) {
                    home_cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, home, home_cap);
                    if (!tmp) { mem_free(home); return 0; }
                    home = tmp;
                }
                home[home_count++] = cand;
            }
        }
        ++candidate;
    }

    return home[n - 1];
}
