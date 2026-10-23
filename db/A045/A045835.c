/*
 * A045835  Numbers n such that sopfr(Omega(n)) = Omega(sopfr(n)), where
 *          Omega(m) is the number and sopfr(m) is the sum of prime factors of
 *          m, with repetition respectively.
 * https://oeis.org/A045835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045835: numbers n such that sopfr(Omega(n)) = Omega(sopfr(n))
   Computation: naively test each integer, caching the results for
   arbitrary‑order calls.  This is not a closed formula but satisfies
   all requirements. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for the sequence generation */
static seq_t *seq_values = NULL;   /* stored terms */
static idx_t  seq_len   = 0;       /* how many terms stored */
static idx_t  seq_cap   = 0;       /* allocated capacity */

/* Smallest‑prime‑factor table */
static uint32_t *spf      = NULL; /* table of smallest prime factor */
static idx_t     spf_cap  = 0;    /* current table size */

/* Build a smallest‑prime‑factor table up to `limit` (inclusive). */
static void build_spf(idx_t limit)
{
    spf = (uint32_t*)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
    spf_cap = limit;
}

/* Ensure that the spf table covers at least `limit`. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_cap) return;
    idx_t new_limit = limit;
    if (spf_cap > 0 && new_limit < spf_cap * 2) new_limit = spf_cap * 2;
    if (new_limit < limit) new_limit = limit;
    build_spf(new_limit);
}

/* Compute Ω(n) and sopfr(n) using the spf table.
   The caller must guarantee that n <= spf_cap. */
static void compute_omega_sopfr(uint64_t n, uint64_t *omega, uint64_t *sopfr)
{
    uint64_t x = n;
    uint64_t o = 0, s = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        while ((x % p) == 0) {
            ++o;
            s += p;
            x /= p;
        }
    }
    *omega  = o;
    *sopfr  = s;
}

/* Sum of prime factors (with multiplicity) of m. */
static uint64_t sopfr_uint(uint64_t m)
{
    uint64_t sum = 0;
    while (m > 1) {
        uint32_t p = spf[m];
        while ((m % p) == 0) {
            sum += p;
            m /= p;
        }
    }
    return sum;
}

/* Count of prime factors (with multiplicity) of m. */
static uint64_t omega_uint(uint64_t m)
{
    uint64_t cnt = 0;
    while (m > 1) {
        uint32_t p = spf[m];
        while ((m % p) == 0) {
            ++cnt;
            m /= p;
        }
    }
    return cnt;
}

/* Test whether n satisfies the defining property of the sequence. */
static bool is_special(uint64_t n)
{
    uint64_t omega_n, sopfr_n;
    compute_omega_sopfr(n, &omega_n, &sopfr_n);
    uint64_t sopfr_omega = sopfr_uint(omega_n);
    uint64_t omega_sopfr = omega_uint(sopfr_n);
    return sopfr_omega == omega_sopfr;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static uint64_t candidate = 1; /* next integer to test */

    while (seq_len < n) {
        if (candidate > (uint64_t)spf_cap) {
            ensure_spf(candidate);
        }
        if (is_special(candidate)) {
            if (seq_len == seq_cap) {
                idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_values = (seq_t*)realloc(seq_values,
                                             (size_t)(new_cap * sizeof(seq_t)));
                if (!seq_values) abort();
                seq_cap = new_cap;
            }
            seq_values[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return seq_values[n - 1];
}
