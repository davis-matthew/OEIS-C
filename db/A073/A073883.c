/*
 * A073883  a(1)=1. For n > 1, a(n) = the smallest positive integer not
 *          included earlier such that the n-th partial sum is n times a
 *          prime.
 * https://oeis.org/A073883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer not used earlier such that
 *            * the sum of the first n terms is n times a prime. (Sequence
 *            A073883)
 * Range:     u8 n <= 13, u16 n <= 440, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 76 terms through n = 1949 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* External types provided by the build */
extern seq_t a(idx_t n);

/* Prime test provided by the utilities */

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

static seq_t *seq = NULL;          /* sequence array, 1‑based */
static idx_t seq_cap = 0;          /* current capacity of seq */
static idx_t seq_len = 0;          /* number of terms computed */

static unsigned char *used = NULL; /* bitset of used integers (0/1) */
static idx_t used_cap = 0;         /* capacity of used array */

static seq_t sum = 0;              /* cumulative sum of the first seq_len terms */

/* Ensure seq array has at least 'size' elements. */
static void ensure_seq_cap(idx_t size)
{
    if (seq_cap >= size)
        return;
    size_t newcap = seq_cap ? seq_cap * 2 : 8;
    if (newcap < size)
        newcap = size;
    seq = mem_resize(seq_t, seq, newcap);
    seq_cap = newcap;
}

/* Ensure used array can index 'idx' (zero‑based). */
static void ensure_used_cap(idx_t idx)
{
    if (used_cap > idx)
        return;
    size_t newcap = used_cap ? used_cap * 2 : 8;
    while (newcap <= (size_t)idx)
        newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, newcap);
    memset(tmp + used_cap, 0, newcap - used_cap);
    used = tmp;
    used_cap = newcap;
}

/* Main function required by the interface. */
seq_t a(idx_t n)
{
    /* Initialize structures on first call. */
    if (seq_len == 0) {
        ensure_seq_cap(2);           /* room for a[1] and index base */
        seq[1] = 1;
        seq_len = 1;
        sum = 1;
        ensure_used_cap(1);
        used[1] = 1;
    }

    /* Compute terms up to n if necessary. */
    while (seq_len < n) {
        idx_t cur = seq_len + 1;                /* index of new term    */
        seq_t prev_sum = sum;                   /* sum before adding   */
        seq_t cand = 1;                          /* candidate value      */

        /* Find smallest unused integer that works. */
        while (1) {
            ensure_used_cap((idx_t)cand);
            if (used[(size_t)cand])
                { ++cand; continue; }

            seq_t new_sum = prev_sum + cand;
            /* new_sum must be divisible by cur, quotient prime */
            if (new_sum % (seq_t)cur == 0) {
                seq_t p = new_sum / (seq_t)cur;
                if (is_prime_big(p)) {
                    /* Accept candidate */
                    ensure_seq_cap(cur + 1);
                    seq[cur] = cand;
                    sum = new_sum;
                    used[(size_t)cand] = 1;
                    break;
                }
            }
            ++cand;
        }

        seq_len = cur;
    }

    return seq[n];
}
