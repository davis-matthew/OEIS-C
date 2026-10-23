/*
 * A102467  Positive integers k such that d(k) <> Omega(k) + omega(k), where d
 *          = A000005, Omega = A001222 and omega = A001221.
 * https://oeis.org/A102467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102467 – numbers k such that d(k) ≠ Ω(k)+ω(k)
   (i.e., k is neither a prime power nor a semiprime). */

/* This implementation sieves the smallest prime factor (SPF) up to
   the current integer and uses the factorisation to test the
   condition.  It works with both C11 and C++17 builds.  In C++ builds
   a std::vector is used for the sequence array, which handles
   construction of arbitrary‑precision seq_t objects.  In C builds
   plain malloc/realloc are used.  All other helpers are included from
   the standard headers.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* --- Type aliases supplied by the build -------------------------------- */
#ifndef seq_t
/* seq_t is supplied by the build (e.g. uint8_t … uint64_t, __int128,
   arbitrary‑precision class).  */
#endif
#ifndef idx_t
    typedef uint64_t idx_t;   /* the index type */
#endif

/* --- Static state ----------------------------------------------------- */
static bool            initialized = false;
static idx_t           current_k   = 0;        /* last integer examined   */

static uint64_t        sieve_limit = 0;      /* maximum value of spf    */
static uint32_t       *spf          = NULL;    /* smallest prime factor   */

static idx_t           seq_len = 0;           /* number of terms stored  */
static idx_t           seq_cap = 0;           /* capacity of seq        */
#if defined(__cplusplus)
static std::vector<seq_t> seq_vec;             /* sequence array (C++)    */
#else
static seq_t          *seq   = NULL;          /* sequence array (C)      */
#endif

/* --- Helper: extend the SPF array up to at least [limit] ---------------- */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    uint64_t new_limit = sieve_limit == 0 ? 256ULL : sieve_limit << 1;
    while (new_limit < limit) new_limit <<= 1;

    uint32_t *new_spf = (uint32_t *)calloc((size_t)(new_limit + 1), sizeof(uint32_t));
    if (!new_spf) exit(1);          /* out of memory */

    new_spf[1] = 1;
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime      */
            new_spf[i] = i;
            if ((uint64_t)i * i <= new_limit) {
                for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                    if (!new_spf[(size_t)j]) new_spf[(size_t)j] = i;
            }
        }
    }
    if (spf) free((void *)spf);
    spf        = new_spf;
    sieve_limit = new_limit;
}

/* --- Helper: generate sequence terms until seq_len >= n  ---------------- */
static void ensure_terms(idx_t n)
{
    if (!initialized) {
        initialized = true;
#if defined(__cplusplus)
        seq_vec.clear();
        seq_vec.reserve(16);
        seq_cap = 16;
#else
        seq_cap = 16;
        seq = (seq_t *)malloc((size_t)seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
#endif
        seq_len = 0;
        current_k = 0;
    }

    while (seq_len < n) {
        ++current_k;
        if ((uint64_t)current_k > sieve_limit) ensure_sieve((uint64_t)current_k);

        /* Factor (current_k) using SPF.  Compute omega and Omega. */
        uint64_t temp = current_k;
        uint64_t omega = 0;   /* distinct prime factors */
        uint64_t Omega = 0;   /* total prime factors */
        while (temp > 1) {
            uint32_t p = spf[(size_t)temp];
            uint64_t count = 0;
            while (temp % p == 0) {
                temp /= p;
                ++count;
            }
            Omega += count;
            ++omega;
        }

        bool include = false;
        if (current_k == 1) {
            include = true;
        } else {
            /* Condition: not a prime power (omega != 1) and not a semiprime (Omega != 2). */
            if (omega != 1 && Omega != 2) include = true;
        }

        if (include) {
#if defined(__cplusplus)
            seq_vec.push_back((seq_t)current_k);
#else
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc((void *)seq, (size_t)seq_cap * sizeof(seq_t));
                if (!seq) exit(1);
            }
            seq[seq_len] = (seq_t)current_k;
#endif
            ++seq_len;
        }
    }
}

/* --- Public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset is 1; for safety */
    ensure_terms(n);
    seq_t result;
#if defined(__cplusplus)
    result = seq_vec[n - 1];
#else
    result = seq[n - 1];
#endif
    return result;
}
