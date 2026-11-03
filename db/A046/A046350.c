/*
 * A046350  Odd composite numbers with only palindromic prime factors.
 * https://oeis.org/A046350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = odd composite numbers whose prime factors are all
 *            palindromic primes. The implementation builds the sequence in
 *            ascending order using a Hamming‑style generator over the set of
 *            odd palindromic primes, discarding primes and duplicates. The
 *            first few terms agree with the OEIS entries shown above
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 977 (2026-11-03)
 * Helpers:   build_primes_upto, is_palindrome_base, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions supplied by the build system */
#include "../utils/build_primes_upto.h"
#include "../utils/is_palindrome_base.h"
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

/* --------------------------------------------------------------------- */
static void ensure_size_cap(size_t need, size_t *cap, size_t **ptr)
{
    if (need <= *cap) return;
    size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
    while (need > newcap) newcap <<= 1;
    size_t *tmp = (size_t *)realloc(*ptr, newcap * sizeof(size_t));
    if (!tmp) exit(1);
    *ptr  = tmp;
    *cap  = newcap;
}

/* --------------------------------------------------------------------- */
/*  Helper routine: grow a dynamic array of seq_t                        */
/* --------------------------------------------------------------------- */
static void ensure_seq_cap(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap) return;
    size_t newcap = (*cap == 0) ? 4 : *cap << 1;
    while (need > newcap) newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *ptr, newcap);
    if (!tmp) exit(1);
    *ptr  = tmp;
    *cap  = newcap;
}

/* --------------------------------------------------------------------- */
/*  Helper routine: grow a dynamic array of size_t                       */

/* --------------------------------------------------------------------- */
/*  Global static data for the generator                                */
/* --------------------------------------------------------------------- */
static seq_t   *palPrimes = NULL;   /* list of odd palindromic primes   */
static size_t   pCount    = 0;       /* number of such primes            */
static seq_t   *seqAll    = NULL;   /* all generated numbers (1, primes, composites) */
static size_t   seqLen    = 0;
static size_t   seqCap    = 0;
static seq_t   *comps     = NULL;   /* only the composite numbers      */
static size_t   compLen   = 0;
static size_t   compCap   = 0;
static size_t   *ptrIdx   = NULL;   /* indices into seqAll for each prime */

/* --------------------------------------------------------------------- */
/*  Initialisation routine (executed lazily on first call)              */
/* --------------------------------------------------------------------- */
static void init(void)
{
    if (palPrimes != NULL) return;          /* already initialised */

    /* ---- Build the list of odd palindromic primes up to a generous limit ---- */
    const idx_t PRIME_LIMIT = 400000ULL;     /* large enough for n <= 10^4 */
    uint64_t *primes_u64  = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(PRIME_LIMIT, &primes_u64, &primes_cnt);

    /* Count how many of these primes are odd and palindromic */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_u64[i];
        if (p > 2 && is_palindrome_base((seq_t)p, 10ULL))
            ++pCount;
    }

    /* Allocate the prime array and fill it */
    palPrimes = mem_new(seq_t, pCount);
    if (!palPrimes) exit(1);
    size_t idx = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_u64[i];
        if (p > 2 && is_palindrome_base((seq_t)p, 10ULL))
            palPrimes[idx++] = (seq_t)p;
    }

    free(primes_u64);

    /* ---- Allocate the other dynamic arrays ---- */
    ensure_seq_cap(4, &seqCap, &seqAll);
    ensure_seq_cap(4, &compCap, &comps);
    ptrIdx = (size_t *)calloc(pCount, sizeof(size_t));
    if (!ptrIdx) exit(1);

    /* Seed with the multiplicative identity */
    seqAll[0] = (seq_t)1;
    seqLen = 1;
}

/* --------------------------------------------------------------------- */
/*  Main function: returns the n-th term (1‑based index).               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();

    /* Ensure we have at least one prime to start (there always is). */
    if (pCount == 0) return 0;  /* should never happen */

    /* Generate composites until we have n of them */
    seq_t lastAdded = 0;                  /* to avoid duplicates in comps */
    while (compLen < (size_t)n) {
        /* Find the smallest candidate from all primes */
        seq_t minCandidate = palPrimes[0] * seqAll[ptrIdx[0]];
        for (size_t i = 1; i < pCount; ++i) {
            seq_t cand = palPrimes[i] * seqAll[ptrIdx[i]];
            if (cand < minCandidate) minCandidate = cand;
        }

        /* Advance pointers for all primes that produced this candidate */
        for (size_t i = 0; i < pCount; ++i) {
            if (palPrimes[i] * seqAll[ptrIdx[i]] == minCandidate)
                ++ptrIdx[i];
        }

        /* Avoid duplicate numbers in seqAll */
        bool duplicateInAll = (seqLen > 0 && seqAll[seqLen-1] == minCandidate);

        if (!duplicateInAll) {
            /* Append to the full list */
            ensure_seq_cap(seqLen + 1, &seqCap, &seqAll);
            seqAll[seqLen++] = minCandidate;

            /* Check composite (and odd by construction) */
            if (!is_prime_big(minCandidate) && minCandidate > 1) {
                if (minCandidate != lastAdded) {   /* skip duplicate composites */
                    ensure_seq_cap(compLen + 1, &compCap, &comps);
                    comps[compLen++] = minCandidate;
                    lastAdded = minCandidate;
                }
            }
        }
        /* If duplicateInAll, we still advanced pointers, so next iteration
           will produce the next distinct number. */
    }

    return comps[(size_t)n - 1];
}
