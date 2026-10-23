/*
 * A164901  a(1)=1, a(2) = 2. For n >=3, a(n) = the smallest integer > a(n-1)
 *          that is coprime to every sum of any two distinct earlier terms of
 *          this sequence.
 * https://oeis.org/A164901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n≥3, a(n) = the smallest integer > a(n‑1)
 *            that is coprime to every sum of any two distinct earlier terms
 * Range:     u8 n <= 32, u16 n <= 3500, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence on demand using a prime–factorisation approach.  A global cache of forbidden prime factors is maintained. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* Constants and global tables -------------------------------- */
#define SPFLIMIT 2000000U

static seq_t *seq = NULL;          /* sequence terms */
static size_t seq_len = 0;          /* number of terms stored */

/* Forbidding table: forbidden[p] == true ⇔ p divides some pairwise sum seen so far */
static bool *forbidden = NULL;
static size_t forbidden_size = 0;

/* Smallest prime factor table up to SPFLIMIT */
static uint32_t *spf = NULL;
static size_t spf_size = 0;

/* List of primes up to SPFLIMIT used for trial division */
static int *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

static bool initialized = false;

/* ------------------------------------------------------------ */
/* Ensure forbidden array can hold index p */
static void ensure_forbidden_size(size_t p)
{
    if (p < forbidden_size) return;
    size_t new_size = forbidden_size ? forbidden_size : 32;
    while (new_size <= p) new_size <<= 1;
    bool *tmp = (bool *)realloc(forbidden, new_size * sizeof(bool));
    if (!tmp) abort();                 /* out of memory */
    memset(tmp + forbidden_size, 0,
           (new_size - forbidden_size) * sizeof(bool));
    forbidden = tmp;
    forbidden_size = new_size;
}

/* ------------------------------------------------------------ */
/* Mark all distinct prime factors of `sum` as forbidden */
static void add_forbidden_from_sum(seq_t sum)
{
    if (sum < (seq_t)2) return;

    if (sum <= (seq_t)SPFLIMIT) {
        uint32_t u = (uint32_t)sum;
        while (u > 1) {
            uint32_t p = spf[u];
            if (p == 0) p = u;          /* safety – should not happen */
            ensure_forbidden_size((size_t)p);
            forbidden[p] = true;
            while (u % p == 0) u /= p;
        }
    } else {
        uint64_t u = (uint64_t)sum;
        for (size_t i = 0; i < prime_count; ++i) {
            int p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > u) break;
            if (u % (uint64_t)p == 0) {
                ensure_forbidden_size((size_t)p);
                forbidden[p] = true;
                while (u % (uint64_t)p == 0) u /= (uint64_t)p;
            }
        }
        if (u > 1) {                     /* remaining factor is prime */
            size_t p = (size_t)u;
            ensure_forbidden_size(p);
            forbidden[p] = true;
        }
    }
}

/* ------------------------------------------------------------ */
/* Test whether a candidate satisfies the coprimality condition */
static bool candidate_good(seq_t cand)
{
    if (cand < (seq_t)2) return false;

    if (cand <= (seq_t)SPFLIMIT) {
        uint32_t u = (uint32_t)cand;
        while (u > 1) {
            uint32_t p = spf[u];
            if (p == 0) p = u;
            if (forbidden[p]) return false;
            while (u % p == 0) u /= p;
        }
        return true;
    } else {
        uint64_t u = (uint64_t)cand;
        for (size_t i = 0; i < prime_count; ++i) {
            int p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > u) break;
            if (u % (uint64_t)p == 0) {
                if (forbidden[(size_t)p]) return false;
                while (u % (uint64_t)p == 0) u /= (uint64_t)p;
            }
        }
        if (u > 1) {                     /* remaining prime factor */
            size_t p = (size_t)u;
            ensure_forbidden_size(p);
            if (forbidden[p]) return false;
        }
        return true;
    }
}

/* ------------------------------------------------------------ */
/* One–time initialization: build SPF table, primes, and seed */
static void init_once(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate and build linear sieve for SPF and prime list */
    spf_size = (size_t)SPFLIMIT + 1;
    spf = (uint32_t *)calloc(spf_size, sizeof(uint32_t));
    if (!spf) abort();

    prime_cap = (size_t)SPFLIMIT / 10 + 1;   /* rough estimate */
    prime_list = (int *)malloc(prime_cap * sizeof(int));
    if (!prime_list) abort();

    for (uint32_t i = 2; i <= SPFLIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                int *tmp = (int *)realloc(prime_list,
                                          prime_cap * sizeof(int));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_count++] = (int)i;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = prime_list[j];
            uint64_t prod = (uint64_t)i * (uint64_t)p;
            if (prod > SPFLIMIT) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }

    /* Initialize forbidden array */
    forbidden_size = (size_t)SPFLIMIT + 1;
    forbidden = (bool *)calloc(forbidden_size, sizeof(bool));
    if (!forbidden) abort();

    /* Seed sequence with first two terms */
    seq_len = 2;
    seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    add_forbidden_from_sum((seq_t)3);   /* sum of 1 and 2 */
}

/* ------------------------------------------------------------ */
/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    init_once();

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        while (!candidate_good(candidate))
            candidate++;

        /* Append the new term to the sequence */
        seq_t *new_seq = (seq_t *)realloc(seq,
                                          (seq_len + 1) * sizeof(seq_t));
        if (!new_seq) abort();
        seq = new_seq;
        seq[seq_len] = candidate;

        /* Update forbidden set with all new sums involving the new term */
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t sum = candidate + seq[i];
            add_forbidden_from_sum(sum);
        }
        seq_len++;
    }

    return seq[n - 1];
}
