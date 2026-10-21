/*
 * A080454  Smallest multiple of n beginning with the backward concatenation
 *          of numbers from (n-1) to 1.
 * https://oeis.org/A080454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A080454
 * Formula: a(n) is the smallest multiple of n whose decimal representation
 * starts with the reverse concatenation of the numbers (n‑1),(n‑2),… ,1.
 *
 * Algorithm:
 *   1. Compute the value of the reverse prefix modulo n and its length.
 *   2. Find the smallest k such that
 *          target = (n – (prefix * 10^k mod n)) mod n  < 10^k .
 *      The required suffix is exactly target.
 *   3. Build the final number by concatenating the prefix again and
 *      appending the k‑digit (zero‑padded) representation of target.
 *   4. Cache results for n ≤ 10000 to avoid recomputation.
 *
 * The code works for any idx_t ≥ 1.  For seq_t small (e.g. uint8_t) the
 * arithmetic may overflow, but values that fit in seq_t are produced
 * exactly.  For larger seq_t (e.g. arbitrary‑precision) the result is
 * computed exactly.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- helpers ---------- */
/* return number of decimal digits of x (x ≥ 1) */
static inline int digit_len(idx_t x)
{
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* ---------- cache for small n ---------- */
static seq_t cache[10001];
static bool present_cache[10001];

/* ---------- powers of ten for seq_t (up to 20 digits) ---------- */
static bool pow10_init = false;
static seq_t pow10_seq[21];       /* 10^d as seq_t (may overflow) */

/* initialise pow10_seq once */
static void init_pow10_seq(void)
{
    pow10_seq[0] = (seq_t)1;
    for (int i = 1; i <= 20; ++i)
        pow10_seq[i] = pow10_seq[i - 1] * (seq_t)10;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1)
        return (seq_t)0;

    /* initialise static helpers on first entry */
    static bool first = true;
    if (first) {
        memset(present_cache, 0, sizeof(present_cache));
        init_pow10_seq();
        first = false;
    }

    /* use cache if available */
    if (n <= 10000 && present_cache[n])
        return cache[n];

    /* ---------- step 1: prefix modulo n and length ---------- */
    idx_t pow10_mod[21];      /* 10^d mod n */
    pow10_mod[0] = 1 % n;
    for (int i = 1; i <= 20; ++i)
        pow10_mod[i] = (pow10_mod[i - 1] * 10) % n;

    idx_t prefix_mod = 0;     /* modulo n */
    int prefix_len = 0;

    for (idx_t m = n - 1; m > 0; --m) {
        int d = digit_len(m);
        prefix_len += d;
        /* (prefix_mod * 10^d + m) mod n */
        unsigned __int128 prod = (unsigned __int128)prefix_mod * pow10_mod[d];
        prefix_mod = (idx_t)((prod + (m % n)) % n);
    }

    /* ---------- step 2: find minimal k and suffix value ---------- */
    idx_t pow10k_mod = 1 % n;           /* 10^k mod n */
    unsigned __int128 pow10k_val128 = 1;/* 10^k as 128‑bit for comparison */
    seq_t pow10k_val_seq = (seq_t)1;     /* 10^k as seq_t (with overflow) */
    int k = 0;
    idx_t target = 0;

    while (true) {
        unsigned __int128 prod = (unsigned __int128)prefix_mod * pow10k_mod;
        idx_t temp = (idx_t)(prod % n);          /* prefix * 10^k mod n */
        target = (n - temp) % n;
        if ((idx_t)target < (idx_t)pow10k_val128)   /* target < 10^k? */
            break;
        /* increase k */
        ++k;
        pow10k_mod = (pow10k_mod * 10) % n;
        pow10k_val128 *= 10;
        pow10k_val_seq = pow10k_val_seq * (seq_t)10;/* may overflow */
    }

    /* ---------- step 3: build full number ---------- */
    seq_t res_prefix = 0;
    for (idx_t m = n - 1; m > 0; --m) {
        int d = digit_len(m);
        res_prefix = res_prefix * pow10_seq[d] + (seq_t)m;
    }
    seq_t res = res_prefix * pow10k_val_seq + (seq_t)target;

    /* ---------- cache and return ---------- */
    if (n <= 10000) {
        cache[n] = res;
        present_cache[n] = true;
    }
    return res;
}
