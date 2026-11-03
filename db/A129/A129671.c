/*
 * A129671  Right truncatable primes in base 5 (written in decimal form).
 * https://oeis.org/A129671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: full, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute all right‑truncatable primes in base‑5 (written in decimal)
   by depth‑first search of the base‑5 tree. The resulting list is
   sorted increasingly, and a(idx_t n) returns the nth element (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
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

/* Comparator used by qsort.  Works for unsigned integral types as
   well as for arbitrary‑precision types that overload operator< and
   operator>. */
static int cmp_seq_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Static storage for the finished sequence. */
static seq_t  *a_values      = NULL;
static size_t a_count        = 0;   /* number of stored numbers */
static size_t a_capacity     = 0;   /* allocated capacity */
static bool    a_initialized = false;

/* Depth‑first search: try to append each base‑5 digit (0‥4) to the
   current value.  If the result is prime, add it to the list and
   continue the search from that value. */
static void dfs(seq_t val)
{
    for (int d = 0; d <= 4; ++d) {
        seq_t new_val = val * (seq_t)5 + (seq_t)d;
        if (is_prime_big(new_val)) {
            append_candidate(&a_values, &a_count, &a_capacity, new_val);
            dfs(new_val);
        }
    }
}

/* Build the full finite set of right‑truncatable primes in base‑5.
   The one‑digit base‑5 primes are the only entry points of the search. */
static void build_sequence(void)
{
    seq_t first[2] = {(seq_t)2, (seq_t)3};

    for (size_t i = 0; i < 2; ++i) {
        seq_t v = first[i];
        append_candidate(&a_values, &a_count, &a_capacity, v);
        dfs(v);
    }

    /* Sort the array into increasing order. */
    mem_qsort(a_values, a_count, sizeof(seq_t), cmp_seq_seq);
}

/* Public interface: a(n) returns the nth term (1‑based) of A129671. */
seq_t a(idx_t n)
{
    if (!a_initialized) {
        build_sequence();
        a_initialized = true;
    }

    if (n == 0)                /* undefined for n = 0, return 0 */
        return (seq_t)0;
    if ((idx_t)(n - 1) >= a_count)
        return (seq_t)0;        /* n larger than the number of known terms */

    return a_values[n - 1];
}
