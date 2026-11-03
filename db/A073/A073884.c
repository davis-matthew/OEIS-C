/*
 * A073884  a(1)=2. Let S(k) = sum of k terms. If S(n) is prime then a(n+1) is
 *          the smallest prime not included earlier else a(n+1) is the
 *          smallest composite number not included earlier.
 * https://oeis.org/A073884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 646, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8207 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073884.  a(1)=2.  For n>1, let S(n) be the sum of the first n terms.
    If S(n) is prime, a(n+1) is the smallest prime that has not
    appeared earlier; otherwise a(n+1) is the smallest composite
    that has not appeared earlier.  The sequence is computed on demand
    and cached, so calls may be in any order.  */

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

/* State for memoisation (static, external linkage). */
static seq_t *seq      = NULL;   /* array of computed terms   */
static size_t seq_cap  = 0;      /* capacity of seq           */
static size_t seq_len  = 0;      /* number of terms stored    */
static seq_t sum       = 0;      /* sum of the stored terms   */

static bool *used      = NULL;   /* bool array for used values*/
static size_t used_cap = 0;      /* capacity of used array    */

/* Next candidates for prime/composite search. */
static seq_t next_prime_candidate    = 3;
static seq_t next_composite_candidate = 4;

/* Ensure that the array `seq` can hold at least `need` elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap : 8;
    while (cap < need) cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, seq, cap);
    if (!tmp) abort();
    seq = tmp;
    seq_cap = cap;
}

/* Ensure that the array `used` can be indexed by `idx` at least. */
static void ensure_used_capacity(idx_t idx)
{
    if ((size_t)idx < used_cap) return;
    size_t cap = used_cap ? used_cap : 16;
    while (cap <= (size_t)idx) cap <<= 1;
    bool *tmp = (bool *)realloc(used, cap * sizeof(bool));
    if (!tmp) abort();
    /* initialise the new part of the array to false */
    for (size_t i = used_cap; i < cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = cap;
}

/* Initialise state on first call. */
static void init_state(void)
{
    if (seq) return;   /* already initialised */

    /* sequence array */
    seq_cap = 8;
    seq = mem_new(seq_t, seq_cap);
    if (!seq) abort();

    /* used array */
    used_cap = 16;
    used = (bool *)calloc(used_cap, sizeof(bool));
    if (!used) abort();

    /* first term a(1)=2 */
    seq[0] = 2;
    seq_len = 1;
    sum = 2;
    ensure_used_capacity(2);
    used[2] = true;

    /* initialise candidate trackers */
    next_prime_candidate = 3;
    next_composite_candidate = 4;
}

/* Generate terms up to index `n` (1‑based). */
static void generate_up_to(idx_t n)
{
    init_state();

    while (seq_len < (size_t)n) {
        seq_t cand;

        /* decide based on current sum */
        if (is_prime_big(sum)) {           /* need a prime   */
            cand = next_prime_candidate;
            while (true) {
                idx_t cand_idx = (idx_t)cand;
                ensure_used_capacity(cand_idx);
                if (!used[cand_idx] && is_prime_big(cand)) break;
                ++cand;
            }
            next_prime_candidate = cand + 1;
        } else {                            /* need a composite */
            cand = next_composite_candidate;
            while (true) {
                idx_t cand_idx = (idx_t)cand;
                ensure_used_capacity(cand_idx);
                if (!used[cand_idx] && !is_prime_big(cand) && cand >= 4) break;
                ++cand;
            }
            next_composite_candidate = cand + 1;
        }

        /* store the chosen candidate */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = cand;
        sum += cand;

        /* mark as used */
        idx_t cand_idx = (idx_t)cand;
        ensure_used_capacity(cand_idx);
        used[cand_idx] = true;
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1 – safe guard */
    generate_up_to(n);
    return seq[n - 1];
}
