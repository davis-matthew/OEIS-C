/*
 * A067526  Numbers n such that n - 2^k is a prime or 1 for all k satisfying 0
 *          < k, 2^k < n.
 * https://oeis.org/A067526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) is the nth positive integer n such that for every k with 0
 *            < k and 2^k < n, the number n-2^k is either 1 or a prime
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2_i32.h"
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

/* Return the n-th term of OEIS sequence A067526.  */
seq_t a(idx_t n)
{
    /* Static state to allow non‑monotonic calls.  */
    static seq_t *seq  = NULL;      /* array of found terms   */
    static size_t seq_len = 0;      /* number of stored terms */
    static size_t seq_cap = 0;      /* allocated capacity    */
    static seq_t last_checked = (seq_t)2;  /* last n that was examined */

    /* Allocate the array on first use.  */
    if (seq == NULL) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        if (seq == NULL) return (seq_t)0;  /* allocation failure */
    }

    /* Generate terms until we have at least n of them.  */
    while (seq_len < n) {
        seq_t candidate = last_checked + 1;
        last_checked   = candidate;

        bool good = true;
        /* Test all powers of two less than candidate.  */
        for (int k = 1; ; ++k) {
            seq_t p2 = pow2_i32(k);
            if (!(p2 < candidate))       /* stop when 2^k >= candidate */
                break;

            seq_t diff = candidate - p2;
            if (diff != (seq_t)1) {       /* 1 is always acceptable */
                if (!is_prime_big(diff)) { /* otherwise must be prime */
                    good = false;
                    break;
                }
            }
        }

        if (good) {
            /* Store the new term.  */
            append_candidate(&seq, &seq_len, &seq_cap, candidate);
        }
    }

    return seq[n - 1];  /* a(1) is the first element stored.  */
}
