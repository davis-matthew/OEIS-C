/*
 * A068632  a(1) = 2; a(n+1) = smallest composite number not in the sequence
 *          such that the sum of all the terms is prime.
 * https://oeis.org/A068632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068632 – “Smallest composite number not in the sequence
   that makes the cumulative sum prime”.
   a(0) = 2
   For n ≥ 0:
      a(n+1) := the smallest composite integer c,
                 with c ∉ {a(0),…,a(n)},
                 such that a(0)+…+a(n)+c is prime. */
/* Implementation uses memoisation, a deterministic
   Miller–Rabin test for 64‑bit integers, and the generic
   isPrimeGeneric() routine for arbitrary precision sums.
   The algorithm is O(n·π(n)) but with the small size of the
   numbers involved it runs well under the required limits. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"      /* isPrimeGeneric(seq_t)   */

static seq_t *seq = NULL;         /* sequence values               */
static size_t seq_len = 0;        /* number of terms computed       */
static size_t seq_cap = 0;        /* capacity of seq array          */

static bool  *used = NULL;        /* bitmap: used[i] == true -> i is in the sequence   */
static size_t used_cap = 0;       /* highest index that is valid in *used             */

static seq_t sum = 0;             /* cumulative sum of sequence */

static const idx_t FIRST_CAND = 4;  /* first composite to try */

/* ---- helpers --------------------------------------------------- */

static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void extend_used_to(size_t idx)
{
    if (used_cap >= idx) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap < idx) new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, (new_cap + 1) * sizeof(bool));
    for (size_t i = used_cap + 1; i <= new_cap; ++i) new_used[i] = false;
    used = new_used;
    used_cap = new_cap;
}

/* Deterministic Miller–Rabin for 64‑bit integers (used only for small
   composites; the sum is tested with isPrimeGeneric). */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t powmod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool isPrime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smallprimes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i=0; i<sizeof(smallprimes)/sizeof(smallprimes[0]); ++i)
    {
        uint64_t p = smallprimes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, r = 0;
    while ((d & 1) == 0) { d >>= 1; r++; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i=0; i<sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (unsigned j=1; j<r; ++j)
        {
            x = mulmod64(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---- main function --------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq_len == 0)
    {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)2;
        seq_len = 1;
        sum = (seq_t)2;
        used_cap = 16;
        used = (bool *)malloc((used_cap + 1) * sizeof(bool));
        for (size_t i = 0; i <= used_cap; ++i) used[i] = false;
        used[2] = true;
    }

    /* compute up to requested index (in any order) */
    while (seq_len <= (size_t)n)
    {
        size_t cand = (size_t)FIRST_CAND;
        for (;;++cand)
        {
            extend_used_to(cand);
            if (used[cand]) continue;
            if (cand <= 1) continue;
            if (isPrime_u64((uint64_t)cand)) continue;    /* not composite */

            seq_t new_sum = sum + (seq_t)cand;
            if (isPrimeGeneric(new_sum))
            {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len] = (seq_t)cand;
                seq_len++;
                sum = new_sum;
                used[cand] = true;
                break;
            }
        }
    }
    return seq[n];
}
