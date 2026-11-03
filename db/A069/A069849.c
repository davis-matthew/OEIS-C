/*
 * A069849  Number of distinct primes obtained by inserting an 8 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct primes obtained by inserting an 8 at
 *            all positions in the decimal representation of n (including
 *            prefixing and suffixing)
 * Range:     u8 n <= 14, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
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

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Number of distinct primes obtained by inserting an 8
   into the decimal representation of n. */
seq_t a(idx_t n)
{
    /* convert n to decimal string */
    char s[32];
    int len = snprintf(s, sizeof(s), "%llu", (unsigned long long)n);
    if (len <= 0) return 0;          /* numeric error – should not happen */

    /* store primes found */
    seq_t primes[16];                /* enough for any reasonable number of digits */
    size_t prime_cnt = 0;

    for (int pos = 0; pos <= len; ++pos) {
        /* build the candidate string with an '8' inserted at position pos */
        char cand[32];
        memcpy(cand, s, pos);                /* copy prefix */ 
        cand[pos] = '8';
        memcpy(cand + pos + 1, s + pos, len - pos); /* copy suffix */
        cand[len + 1] = '\0';

        /* convert candidate to seq_t */
        seq_t num = seq_from_str(cand);

        /* test primality */
        if (!is_prime_big(num))
            continue;

        /* check if already counted */
        bool duplicate = false;
        for (size_t k = 0; k < prime_cnt; ++k) {
            if (cmp_seq_t(&num, &primes[k]) == 0) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            primes[prime_cnt++] = num;
        }
    }

    return (seq_t)prime_cnt;
}
