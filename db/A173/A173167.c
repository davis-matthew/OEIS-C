/*
 * A173167  Values n such that n and n+1 are both in A037073.
 * https://oeis.org/A173167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173167: a(n) are the lower terms of consecutive pairs in A037073.
   A037073 consists of integers m such that both 18*m*m-1 and 18*m*m+1 are prime.
   The function builds this sequence on demand. */
#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *a_seq = NULL;          /* dynamically grown array holding a(1..size) */
static size_t a_seq_size = 0;        /* current number of elements stored */
static size_t a_seq_capacity = 0;    /* allocated capacity */

static idx_t last_checked = 0;       /* last n examined for membership in A037073 */
static idx_t last_valid = 0;         /* last n that passed the A037073 test */

/* Ensure that a_seq has capacity at least needed. */
static void ensure_capacity(size_t needed)
{
    if (needed <= a_seq_capacity)
        return;
    size_t new_cap = a_seq_capacity ? a_seq_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = mem_resize(seq_t, a_seq, new_cap);
    if (!new_ptr) {
        /* In a contest environment, exiting is acceptable. */
        exit(1);
    }
    a_seq = new_ptr;
    a_seq_capacity = new_cap;
}

/* Test whether m is in A037073. */
static inline bool in_A037073(idx_t m)
{
    seq_t mm = (seq_t)m;
    seq_t v1 = (seq_t)18 * mm * mm - (seq_t)1;
    seq_t v2 = (seq_t)18 * mm * mm + (seq_t)1;
    return is_prime_big(v1) && is_prime_big(v2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; a(0) is undefined, return 0 */
    /* Extend the cached sequence until we have at least n terms. */
    while (a_seq_size < (size_t)n) {
        ++last_checked;
        if (in_A037073(last_checked)) {
            if (last_valid != 0 && last_checked == last_valid + (idx_t)1) {
                ensure_capacity(a_seq_size + 1);
                a_seq[a_seq_size++] = (seq_t)last_valid;
            }
            last_valid = last_checked;
        }
    }
    return a_seq[n - 1];
}
