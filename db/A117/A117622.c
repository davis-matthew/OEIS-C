/*
 * A117622  Minesweeper sequence: a(n) is the first nonprime number, k, not
 *          occurring previously in the sequence nor the absolute value of its
 *          first forward difference among the first differences and a(1)=1.
 * https://oeis.org/A117622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117622:  
 * a(1) = 1.  
 * For n > 1, a(n) is the smallest non‑prime integer k such that
 * k has not appeared earlier in the sequence and its absolute
 * difference |k – a(n‑1)| has not appeared earlier as a forward
 * difference.  Each integer k and each difference may be used at
 * most once.  This function generates the sequence lazily,
 * storing previously computed values for arbitrary order of calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* static storage for computed terms and used differences */
static seq_t *seq_vals   = NULL;   /* sequence values   */
static size_t seq_cap    = 0;
static size_t seq_len    = 0;      /* number of terms computed */

static seq_t *diff_vals  = NULL;  /* used forward differences */
static size_t diff_cap   = 0;
static size_t diff_len   = 0;

/* Ensure seq_vals has capacity for at least one more element */
static void ensure_seq_cap(void)
{
    if (seq_len < seq_cap)
        return;
    seq_cap = seq_cap ? (seq_cap << 1) : 1;
    seq_vals = mem_resize(seq_t, seq_vals, seq_cap);
    if (!seq_vals)
        abort();
}

/* Ensure diff_vals has capacity for at least one more element */
static void ensure_diff_cap(void)
{
    if (diff_len < diff_cap)
        return;
    diff_cap = diff_cap ? (diff_cap << 1) : 1;
    diff_vals = mem_resize(seq_t, diff_vals, diff_cap);
    if (!diff_vals)
        abort();
}

/* Check whether a value has already appeared in the sequence */
static bool number_used(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_vals[i] == val)
            return true;
    return false;
}

/* Check whether a difference has already been used */
static bool diff_used(seq_t diff)
{
    for (size_t i = 0; i < diff_len; ++i)
        if (diff_vals[i] == diff)
            return true;
    return false;
}

/* Generate the next term of the sequence */
static void generate_next(void)
{
    seq_t prev = seq_vals[seq_len - 1];
    seq_t cand = (seq_t)2;                /* candidate starts at 2 */

    for (;;) {                           /* loop until a valid k is found */
        /* Skip prime candidates (1 is non‑prime but never considered here) */
        if (is_prime_big(cand))
            ++cand;
        else if (number_used(cand))
            ++cand;
        else {
            seq_t diff = (cand > prev) ? (cand - prev) : (prev - cand);
            if (diff_used(diff))
                ++cand;
            else {
                /* Candidate accepted */
                ensure_seq_cap();
                seq_vals[seq_len++] = cand;

                ensure_diff_cap();
                diff_vals[diff_len++] = diff;
                return;
            }
        }
    }
}

/* Public function implementing a(n) for arbitrary order of calls */
seq_t a(idx_t n)
{
    /* Lazy initialization of the sequence */
    if (!seq_vals) {
        seq_cap    = 128;
        seq_vals   = mem_new(seq_t, seq_cap);
        if (!seq_vals)
            abort();
        seq_len    = 1;
        seq_vals[0] = (seq_t)1;            /* a(1) = 1 */

        diff_cap   = 128;
        diff_vals  = mem_new(seq_t, diff_cap);
        if (!diff_vals)
            abort();
        diff_len   = 0;
    }

    /* Generate terms until we have at least n terms */
    while (seq_len < n)
        generate_next();

    return seq_vals[n - 1];
}
