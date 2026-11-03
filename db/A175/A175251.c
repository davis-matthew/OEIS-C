/*
 * A175251  Composites (A002808) with nonprime (A018252) subscripts.
 * https://oeis.org/A175251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = composite(nonprime(n))
 * where nonprime(n) is the n‑th nonprime number (1,4,6,8,9,10,…)
 * and composite(k) is the k‑th composite number (4,6,8,9,10,…).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static idx_t *nonprimes   = NULL;   /* nonprimes[i] = nonprime(i+1) */
static size_t nonprimes_cnt = 0;
static size_t nonprimes_cap = 0;
static idx_t  next_nonprime = 1;     /* next integer to test for nonprime */

static seq_t *composites   = NULL;   /* composites[i] = composite(i+1) */
static size_t composites_cnt = 0;
static size_t composites_cap = 0;
static idx_t  next_composite = 4;    /* next integer to test for composite */

/* Grow an array of idx_t values. */
static void
grow_idx(idx_t **p, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap) {
        idx_t *tmp = (idx_t *)realloc(*p, new_cap * sizeof(idx_t));
        if (tmp == NULL) abort();
        *p   = tmp;
        *cap = new_cap;
    }
}

/* Ensure that we have at least `need` nonprimes stored. */
static void
ensure_nonprimes(idx_t need)
{
    while (nonprimes_cnt < (size_t)need) {
        idx_t x = next_nonprime;
        if (x == 1) {
            grow_idx(&nonprimes, &nonprimes_cap, nonprimes_cnt + 1);
            nonprimes[nonprimes_cnt++] = x;
        } else {
            if (!is_prime_seq((seq_t)x)) {
                grow_idx(&nonprimes, &nonprimes_cap, nonprimes_cnt + 1);
                nonprimes[nonprimes_cnt++] = x;
            }
        }
        ++next_nonprime;
    }
}

/* Ensure that we have at least `need` composites stored. */
static void
ensure_composites(idx_t need)
{
    while (composites_cnt < (size_t)need) {
        idx_t x = next_composite;
        if (x >= 4) {
            if (!is_prime_seq((seq_t)x)) {
                grow_seq(&composites, &composites_cap, composites_cnt + 1);
                composites[composites_cnt++] = (seq_t)x;
            }
        }
        ++next_composite;
    }
}

/* Compute the OEIS sequence A175251. */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    ensure_nonprimes(n);
    idx_t np = nonprimes[n - 1];          /* n‑th nonprime */

    /* Ensure composites up to index np exist. */
    if (np == 0) return (seq_t)0; /* defensive, shouldn't happen */
    ensure_composites(np);
    return composites[(size_t)(np - 1)];
}
