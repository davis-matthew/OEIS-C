/*
 * A064413  EKG sequence (or ECG sequence): a(1) = 1; a(2) = 2; for n > 2,
 *          a(n) = smallest number not already used which shares a factor with
 *          a(n-1).
 * https://oeis.org/A064413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, look, hear, changed
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A064413 – EKG (ECG) Sequence
 *  a(1)=1, a(2)=2.
 *  For n>2 :  a(n) = smallest unused integer having gcd(a(n‑1), a(n)) > 1.
 *  The implementation builds the sequence lazily, maintaining for every
 *  prime factor of the current last term a pointer to the smallest unused
 *  multiple of that prime.  This allows each term to be found in time
 *  proportional to the number of distinct prime factors of a(n‑1), leading
 *  to an overall O(n log n) algorithm.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ----- Configuration ----- */
#define LIMIT 2000000  /* enough to contain all values up to ~10⁴ terms */

/* ----- Global state ----- */
static seq_t *seq = NULL;          /* sequence terms, 1‑based */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* capacity of seq array */

static bool *used = NULL;          /* used[i] true if integer i was used */
static size_t used_cap = 0;        /* capacity of used array */

static uint32_t spf[LIMIT + 1];    /* smallest prime factor of each number   */
static int primes[150000];         /* list of primes up to LIMIT             */
static size_t primes_cnt = 0;

/* ----- Helper routines ----- */

/* Ensure seq has capacity for at least n (1‑based indices) */
static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < n) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used array covers values up to n (inclusive) */
static void ensure_used_cap(size_t n)
{
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (new_used == NULL) exit(1);                 /* OOM */
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = new_used;
    used_cap = new_cap;
}

/* Build Sieve of Eratosthenes with smallest prime factor array */
static void build_sieve(void)
{
    memset(spf, 0, sizeof(spf));
    for (int i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            int p = primes[j];
            int64_t mult = (int64_t)i * p;
            if (mult > LIMIT) break;
            if (p > spf[i]) break;
            spf[mult] = p;
        }
    }
}

/* Lazy initialisation of sequence data */
static void init_sequence(void)
{
    if (seq_len > 0) return;      /* already initialised */

    build_sieve();

    ensure_seq_cap(4);
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)2;
    seq_len = 2;

    ensure_used_cap(LIMIT + 1);
    used[1] = used[2] = true;
}

/* Compute next term after the current last one */
static void compute_next(void)
{
    uint32_t prev = (uint32_t)seq[seq_len];
    /* Factorise prev using spf (unique factors only) */
    int fac[12];
    size_t fac_cnt = 0;
    uint32_t x = prev;
    while (x > 1) {
        uint32_t p = spf[x];
        fac[fac_cnt++] = (int)p;
        while (x % p == 0) x /= p;
    }

    uint32_t best = UINT32_MAX;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t p = (uint32_t)fac[i];
        uint32_t candidate = p;
        while (candidate <= LIMIT && used[candidate]) candidate += p;
        if (candidate < best) best = candidate;
    }

    /* Fallback: if no prime factor found (should not happen) */
    if (best == UINT32_MAX) best = prev + 1;
    /* Mark used and store the term */
    used[best] = true;
    ensure_seq_cap(seq_len + 2);
    seq[++seq_len] = (seq_t)best;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    init_sequence();
    while ((size_t)n > seq_len) {
        compute_next();
    }
    return seq[n];
}
